package ru.itmo.mit.git.objects;

import org.apache.commons.io.FileUtils;
import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.entities.Index;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;
import ru.itmo.mit.git.utils.MyFileUtils;

import java.io.IOException;
import java.io.Serializable;
import java.nio.file.DirectoryNotEmptyException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Tree extends GitObject implements Serializable {
    /**
     * Construct tree of directories out of index
     */
    public Tree(GitRepo repo) throws GitException, GitFileException {
        super(repo, Hash.generateHash(repo.getIndexPath()),Type.TREE);
        try {
            Files.createDirectory(Hash.getObjectsPrefixDir(repo.getObjectsDir(), hash));
            Files.copy(repo.getIndexPath(), Hash.getObjectDir(repo.getObjectsDir(), this.getHash()));
        } catch (IOException e) {
            System.err.println("Such commit already exist");
        }
    }

    public Tree(GitRepo repo, Hash hash, Type type, String name) {
        super(repo, hash, type, name);
    }

   @Override
   public GitObject restore() throws GitException {
        try {
            MyFileUtils.deleteFiles(repo.getRootDir(), repo.getGitDir());
        } catch (DirectoryNotEmptyException ignored) {

        }
        catch (IOException e) {
            throw new GitFileException("Error occured while cleaning dir", e.getCause());
        }
        recover(repo.readIndex(getPathInObjectsDir()));

        return this;
    }

    public void recover(Index index) throws GitException, GitFileException {
        // restore file from serealized index in tree
        for(var entry : index.get()) {
            Index.Record record = entry.getValue();
            if (record.type == Type.TREE) {
                Path indexToRecover = Hash.getObjectDir(repo.getObjectsDir(), record.hash);
                recover(repo.readIndex(indexToRecover));
            }
            Blob obj = new Blob(repo, Path.of(entry.getKey()), record);
            obj.restore();
        }
    }
    @Override
    public void writeToObjects(GitObject object) throws GitException {
        try {
            Files.copy(repo.getIndexPath(), getPathInObjectsDir());
        } catch (IOException e) {
            throw new GitException("Error occured while writing file tree to objects dir", e.getCause());
        }
    }

}
