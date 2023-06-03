package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.nio.file.Path;
import java.util.List;

public class Rm implements Command {

    GitRepo repo;
    String file;
    public Rm(GitRepo repo, List<String> args) {
        this.repo = repo;
        parse(args);
        repo.getIndex().remove(file);
    }
    @Override
    public void parse(List<String> args) {
        if (args.isEmpty()) {
            return;
        }
        String fileName = args.get(0).split(" ")[0];
        file = fileName.contains(repo.getRootDir().toString()) ? fileName : Path.of(repo.getRootDir().toString(), fileName).toString();
    }

    @Override
    public void run() throws GitException, GitFileException {

    }
}
