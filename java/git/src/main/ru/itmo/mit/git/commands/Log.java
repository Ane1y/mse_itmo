package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.objects.GitCommit;

import java.io.PrintStream;
import java.nio.file.Path;
import java.util.List;

public class Log implements Command {
    GitRepo repo;
    PrintStream printStream;
    Hash revision;

    public Log(GitRepo repo, PrintStream printStream, List<String> args) {
        this.repo = repo;
        this.printStream = printStream;
        parse(args);
    }
    @Override
    public void parse(List<String> args) {
        if (args.isEmpty()) {
            revision = repo.getHead();
        } else {
            var split = args.get(0).split(" ");
            revision = (split.length == 0) ? repo.getHead() : new Hash(split[0]);
        }
    }

    @Override
    public void run() throws GitException {
        if (revision == null) {
            throw new GitException("HEAD not found");
        }
        Path objPath = Hash.getObjectDirByRevision(repo.getObjectsDir(), revision);
        GitCommit headCommit = GitCommit.restoreCommitObj(repo, Hash.getHash(objPath));
        headCommit.printCommit(printStream);
        while (!(headCommit.getRecord().prevCommit() == null)) {
            headCommit = GitCommit.restoreCommitObj(repo, headCommit.getRecord().prevCommit());
            headCommit.printCommit(printStream);
        };
    }
}
