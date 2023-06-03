package ru.itmo.mit.git.commands;


import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.objects.Blob;
import ru.itmo.mit.git.objects.GitObject;
import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.entities.Index;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class Add implements Command {

    private final List<Path> files = new ArrayList<>();
    PrintStream printStream;
    private final GitRepo repo;

    public Add(GitRepo repo, PrintStream printStream, List<String> args) throws GitFileException {
        this.repo = repo;
        this.printStream = printStream;
        parse(args);
    }
    @Override
    public void parse(List<String> args) throws GitFileException {
        for (var arg : args) {
            var path = Path.of(repo.getRootDir().toString(), arg);
            if(!Files.exists(path)) {
                throw new GitFileException(path.toString());
            }
            files.add(path);
        }
    }

    @Override
    public void run() throws GitException, GitFileException {
        Index index = repo.getIndex();
        for(var file : files) {
            GitObject object;
            Hash hash = Hash.generateHash(file);
            if (!index.contains(file.toString())) {
                object = new Blob(repo, file, hash);
            } else {
                Hash oldHash = index.getRecord(file.toString()).hash;
                if (oldHash.equals(hash)) {
                    continue;
                }
                object = new Blob(repo, file, hash);
            }
            index.add(object);
        }
        repo.updateIndex();
        printStream.println("Add completed successful");
    }
}
