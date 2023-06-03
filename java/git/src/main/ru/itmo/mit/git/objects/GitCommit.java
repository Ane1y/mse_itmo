package ru.itmo.mit.git.objects;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Date;

public class GitCommit extends GitObject {
    private final Record record;
    public record Record(Tree dirs, Hash prevCommit, String message, Date date, String author) implements Serializable {
        public Record(Tree dirs, Hash prevCommit, String message, String author) {
            this(dirs, prevCommit, message, new Date(), author);
        }
    }

    public GitCommit(GitRepo repo, Record record, Hash hash) throws GitException {
        // using hash as name (maybe i will fix it later)
        super(repo, hash, Type.COMMIT, hash.hash());
        this.record = record;
    }

    public Record getRecord() {
        return record;
    }
    @Override
    public GitObject restore() {
        return null;
    }

    @Override
    public void writeToObjects(GitObject object) throws GitException, GitFileException {
        if (!(object.getType() == Type.COMMIT)) {
            throw new IllegalArgumentException(String.format("Try to write %s object as commit", object.getType()));
        }
        createCommitObj(repo, record);
    }

    public void printCommit(PrintStream printStream) {
        printStream.println("commit " + hash.hash());
        printStream.println("Author: " + record.author());
        printStream.println("Date:" + record.date());
        printStream.println(record.message);
        printStream.println();
    }

    public static Hash createCommitObj(GitRepo repo, Record record) throws GitException {
        Path tmpPath = Path.of(repo.getGitDir().toString(), "tmp");
        try (var fileOut = new FileOutputStream(tmpPath.toString());
             var outputStream = new ObjectOutputStream(fileOut)) {
            outputStream.writeObject(record);
            Hash hash = Hash.generateHash(tmpPath);
            Files.createDirectory(hash.getObjectFolder(repo.getObjectsDir()));
            Files.move(tmpPath, Hash.getObjectDir(repo.getObjectsDir(), hash));
            return hash;
        } catch (IOException | GitException e) {
            throw new GitException("Error occurred when index was deserializing");
        }
    }

    public static GitCommit restoreCommitObj(GitRepo repo, Hash hash) throws GitException {
        Path objPath = Hash.getObjectDir(repo.getObjectsDir(), hash);
        Record restored;
        try (FileInputStream serializedFile = new FileInputStream(objPath.toString());
             ObjectInputStream in = new ObjectInputStream(serializedFile)) {
            restored = (Record)in.readObject();
        } catch (IOException i) {
            throw new GitFileException("Error occurred while openning commit file to sereealize it");
        } catch (ClassNotFoundException c) {
            throw new RuntimeException("Class GitCommit.Record not found");
        }
        return new GitCommit(repo, restored, hash);
    }
}
