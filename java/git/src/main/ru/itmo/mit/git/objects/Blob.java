package ru.itmo.mit.git.objects;

import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.utils.Hash;
import ru.itmo.mit.git.entities.Index;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class Blob extends GitObject {

    public Blob(GitRepo repo, Path file, Hash hash) throws GitException {
        super(repo, hash, Type.BLOB, file.toString());
        writeToObjects(this);
    }

    public Blob(GitRepo repo, Path file, Index.Record record) throws GitException {
        super(repo, record.hash, Type.BLOB, file.toString());
        writeToObjects(this);
    }

    @Override
    public GitObject restore() throws GitException, GitFileException {
        try {
            Files.createDirectories(Path.of(name).getParent());
            Files.copy(getPathInObjectsDir(), Path.of(name));
        } catch (IOException e) {
            throw new GitFileException(getPathInObjectsDir().toString(), name, e.getCause());
        }
        return this;
    }

    @Override
    public void writeToObjects(GitObject object) throws GitException {
        Path objPath = Hash.getObjectDir(repo.getObjectsDir(), hash);
        // create copy in objects directory
        try {
            if (!Files.exists(objPath)) {
                Files.createDirectory(Hash.getObjectsPrefixDir(repo.getObjectsDir(), hash));
                convertToBlob(Path.of(name), objPath);
            }
        } catch (IOException e) {
            throw new GitException(String.format("Could not create object from file %s", name));
        }
    }

    private void convertToBlob(Path file, Path newFile) throws IOException {
        // вдруг решу сделать какое нибудь классное сжатие
        // (но это вряд ли, потому что я пишу этот комментарий в 23 41 20 мая 2023 года)
        try {
            Files.copy(file, newFile, StandardCopyOption.COPY_ATTRIBUTES);
        } catch (IOException e) {
            throw new IOException("Can not copy file to objects folder", e.getCause());
        }
    }
}
