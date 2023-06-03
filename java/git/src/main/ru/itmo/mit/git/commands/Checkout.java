package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;
import ru.itmo.mit.git.objects.GitCommit;
import ru.itmo.mit.git.utils.Hash;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

public class Checkout implements Command {
    private final GitRepo repo;
    private GitCommit commit;
    private final boolean reset;
    private boolean stop = false;
    public Checkout(GitRepo repo, List<String> args, boolean reset) throws GitException {
        this.reset = reset;
        this.repo = repo;
        parse(args);
    }
    @Override
    public void parse(List<String> args) throws GitException {
        if (args.isEmpty()) {
            throw new GitException("Too few argumets for checkout cmd");
        }
        var split = args.get(0).split(" ");
        String arg = split[0];
        if (arg.equals("master")) {
            commit = GitCommit.restoreCommitObj(repo, repo.getMaster());
        } else if (arg.equals("HEAD")) {
            try {
                int number;
                if (args.size() == 1) {
                    number = Integer.parseInt(split[1]);
                } else {
                    number = Integer.parseInt(args.get(1));
                }
                commit = GitCommit.restoreCommitObj(repo, repo.getHead());
                while (number != 0 && commit.getRecord().prevCommit() != null) {
                    commit = GitCommit.restoreCommitObj(repo,
                            commit.getRecord().prevCommit());
                    number--;
                }

            } catch (IndexOutOfBoundsException e) {
                throw new GitException("HEAD~N must contain number of commit");
            }
        } else if (arg.equals("--")) {
            stop = true;
            Path file = args.get(1).contains(repo.getRootDir().toString()) ?
                    Path.of(args.get(1)) : Path.of(repo.getRootDir().toString(), args.get(1));
            Hash hash = repo.getIndex().getRecord(file.toString()).hash;
            try {
                Files.copy(Hash.getObjectDir(repo.getObjectsDir(), hash), file, REPLACE_EXISTING);
            } catch (IOException e) {
                throw new GitFileException(String.format("Cannot recover file %s", file), e.getCause());
            }
            return;
        }
        else {
            Path path = Hash.getObjectDirByRevision(repo.getObjectsDir(), new Hash(arg));
            commit = GitCommit.restoreCommitObj(repo, Hash.getHash(path));
        }
        repo.setHead(commit.getHash());
    }

    @Override
    public void run() throws GitException {
        if (reset) {
            repo.setMaster(commit.getHash());
            repo.setHead(commit.getHash());
            commit.getRecord().dirs().restore();
        } else {
            if (!stop) {
                commit.getRecord().dirs().restore();
            }
        }
    }
}
