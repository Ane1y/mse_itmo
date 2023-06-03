package ru.itmo.mit.git.objects;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.Serializable;
import java.nio.file.Path;

public abstract class GitObject implements Serializable {
    /**
     * All the derived class creates and writes hash-named copy of themselves in .git/objects/
     */
    public enum Type {
        COMMIT,
        BLOB,
        TREE
    }
    protected final GitRepo repo;
    protected final Hash hash;
    protected final Type type;

    //path for blobs, hashes for commits and trees;
    protected final String name;

    protected GitObject(GitRepo repo, Hash hash, Type type, String name) {
        this.repo = repo;
        this.hash = hash;
        this.type = type;
        this.name = name;
    }

    protected GitObject(GitRepo repo, Hash hash, Type type) {
        this.repo = repo;
        this.hash = hash;
        this.type = type;
        this.name = hash.hash();
    }
    public Type getType() {
        return type;
    }
    public Hash getHash() {
        return hash;
    }

    public String getName() {
        return name;
    }

    public Path getPathInObjectsDir() throws GitException {
        return Hash.getObjectDir(repo.getObjectsDir(), hash);
    }

    public abstract GitObject restore() throws GitException, GitFileException;
    public abstract void writeToObjects(GitObject object) throws GitException, GitFileException;

}
