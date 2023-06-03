package ru.itmo.mit.git.entities;

import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;
import ru.itmo.mit.git.utils.MyFileUtils;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.utils.Serializing;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.List;


public class GitRepo implements Externalizable {
    private Path rootDir;
    private Path gitDir;

    // ветвление не делаю, поэтому в голове будет хранится хеш коммита, а не ссылка
    // ссылок пока тоже нет
    private Hash head;
    private Hash master;
    private String author;
    private Index index;

    public GitRepo() {
    }
    public GitRepo(Path workingDir, boolean init) throws IOException, GitException {
        author = "Ivan Ivanov: ivan@ivanov.com";
        rootDir = workingDir.toAbsolutePath().normalize();
        gitDir = Path.of(rootDir.toString(), ".git");

        if (!Files.exists(gitDir)) {
            if (init) {
                init();
            } else {
                throw new GitException("Git repository not found");
            }
        }

        index = readIndex();

        var headInfo = Files.readAllLines(getHeadFile());
        head = (headInfo.isEmpty()) ? null : new Hash(headInfo.get(0));

        var masterInfo = Files.readAllLines(getMasterPath());
        master = (masterInfo.isEmpty()) ?  null : new Hash(masterInfo.get(0));

    }

    public Path getGitDir() {
        return gitDir;
    }

    public String getAuthor() {
        return author;
    }
    public Path getRootDir() {
        return rootDir;
    }
    public Path getRefDir() throws GitException {
        var path = Path.of(gitDir.toString(), "refs");
        if (!Files.exists(path)) {
            throw new GitException("Can not find ref directory");
        }
        return path;
    }
    public Path getObjectsDir() throws GitException {
        var path = Path.of(gitDir.toString(), "objects");
        if (!Files.exists(path)) {
            throw new GitException("Can not find objects directory");
        }
        return path;
    }
    public Path getIndexPath() {
        return Path.of(gitDir.toString(), Index.getIndexFilename());
    }

    public Path getMasterPath() {
        return Path.of(gitDir.toString(), "master");
    }

    public Index getIndex() {
        return index;
    }

    public Hash getHead() {
        return head;
    }
    public Hash getMaster() {
        return master;
    }

    public void setHead(Hash hash) throws GitFileException {
        head = hash;
        try {
            Files.write(getHeadFile(), head.hash().getBytes());
        } catch (IOException e) {
            throw new GitFileException("HEAD file not found");
        }
    }


    public void setMaster(Hash hash) throws GitFileException {
        master = hash;
        byte[] bytes = master.hash().getBytes();
        try {
            Files.write(getMasterPath(), bytes, StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
        } catch (IOException e) {
            throw new GitFileException("master file not found");
        }
    }


    public Index readIndex(Path indexPath) throws GitFileException {
        return Serializing.readClass(indexPath);
    }

    public Index readIndex() throws GitFileException {
        Path indexPath = Path.of(gitDir.toString(), Index.getIndexFilename());
        File file = new File(indexPath.toString());
        return (file.length() == 0) ? new Index() : readIndex(indexPath);
    }
    public void updateIndex() throws GitFileException {
        Serializing.writeClass(index, getIndexPath());
    }

    public List<Path> getAllFilesInWorkingDir() throws GitFileException {
        return MyFileUtils.getAllFiles(rootDir);
    }

    private Path getHeadFile() {
        return Path.of(gitDir.toString(), "HEAD");
    }

    private void init() throws IOException {
        String[] dirs = { "refs", "objects"};
        String[] files = {"index", "HEAD", "config", "master"};

        Files.createDirectories(gitDir);

        for (var dir : dirs) {
            Files.createDirectories(Path.of(gitDir.toString(), dir));
        }

        for (var file : files) {
            Files.createFile(Path.of(gitDir.toString(), file));
        }
    }


    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeObject(rootDir.toString());
        out.writeObject(gitDir.toString());
        out.writeObject(head);
        out.writeObject(master);
        out.writeObject(author);
        out.writeObject(index);
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        rootDir = Path.of((String) in.readObject());
        gitDir = Path.of((String) in.readObject());
        head = (Hash) in.readObject();
        master = (Hash) in.readObject();
        author = (String) in.readObject();
        index = (Index) in.readObject();
    }
}
