package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;
import ru.itmo.mit.git.objects.GitCommit;
import ru.itmo.mit.git.objects.GitObject;
import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.objects.Tree;

import java.io.IOException;
import java.io.PrintStream;
import java.security.NoSuchAlgorithmException;
import java.util.List;

public class Commit implements Command {
    private final GitRepo repo;
    private String message;

    private final PrintStream printStream;
    private GitObject commit;


    public Commit(GitRepo repo, PrintStream printStream, List<String> args) throws GitException, IOException, NoSuchAlgorithmException, GitFileException {
        this.repo = repo;
        this.printStream = printStream;
        parse(args);
        GitCommit.Record record = new GitCommit.Record(new Tree(repo), repo.getHead(),  message, repo.getAuthor());
        Hash hash = GitCommit.createCommitObj(repo, record);
        commit = new GitCommit(repo, record, hash);
    }
    @Override
    public void parse(List<String> args) {
        message = args.get(0);
    }

    @Override
    public void run() throws GitException {
        if (repo.getHead() != null && !repo.getHead().equals(repo.getMaster())) {
            throw new GitException("You are in detached HEAD position");
        }
        repo.setHead(commit.getHash());
        repo.setMaster(commit.getHash());
        printStream.println("Files committed");
    }

}
