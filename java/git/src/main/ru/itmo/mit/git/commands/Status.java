package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;
import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.entities.Index;

import java.io.PrintStream;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class Status implements Command {

    private final GitRepo repo;

    private final PrintStream printStream;
    public Status(GitRepo repo, PrintStream printStream) {
        this.repo = repo;
        this.printStream = printStream;
    }
    @Override
    public void parse(List<String> args) {
        throw new UnsupportedOperationException();
    }

    @Override
    public void run() throws GitException {
        Index index = repo.getIndex();

        List<String> untracked = new ArrayList<>();
        List<String> changed = new ArrayList<>();
        List<String> commited = new ArrayList<>();

        for(var f : repo.getAllFilesInWorkingDir()) {
            String file = f.toString();
            Hash hash = Hash.generateHash(Path.of(file));
            if (!index.contains(file)) {
                untracked.add(file);
            } else if (index.getRecord(file).hash.equals(hash)) {
                commited.add(file);
            } else {
                changed.add(file);
            }
        }
        if (changed.isEmpty() && untracked.isEmpty()) {
            printStream.println("Everything up to date");
        }

        if (!changed.isEmpty()) {
            printStream.println("Changes not staged for commit:");
            printStream.println("(use \"git add <file>...\" to update what will be committed)\n");
            for (var file : changed) {
                printStream.println("modified: " + file);
            }
        }
        if (!untracked.isEmpty()) {
            printStream.println("Untracked files:");
            for (var file : untracked) {
                printStream.println(file);
            }
        }
    }
}
