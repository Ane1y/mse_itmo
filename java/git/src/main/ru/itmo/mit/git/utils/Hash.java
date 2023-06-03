package ru.itmo.mit.git.utils;

import org.apache.commons.io.FileUtils;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collection;
import java.util.List;

public record Hash (String hash) implements Serializable {

    public Path getObjectFolder(Path objectsDir) {
        return Path.of(objectsDir.toString(), hash.substring(0, 2));
    }
    public static Hash generateHash(Path file) throws GitFileException, GitException {
        //        var file_size = Files.size(file);
        //        List<String> lines = Files.lines(file).map(s -> String.format("header = \"blob %d\\0", file_size) + s).collect(Collectors.toList());
        //        Files.write(file, lines);
        try {
            MessageDigest crypt = MessageDigest.getInstance("SHA-1");
            crypt.reset();
            crypt.update(Files.readAllBytes(file));
            return new Hash(new BigInteger(1, crypt.digest()).toString(16));
        } catch (NoSuchAlgorithmException e) {
            throw new GitException("Some problems with hash generating", e.getCause());
        } catch (IOException e) {
            throw new GitFileException(file.toString(), e.getCause());
        }
    }

    public static Path getObjectDir(Path objectsDir, Hash hash) {
        return Path.of(objectsDir.toString(), hash.hash.substring(0, 2), hash.hash.substring(2));
    }

    public static Hash getHash(Path pathInObjectsDir) {
        String[] split = pathInObjectsDir.toString().split("/");
        int n = split.length;
        return new Hash(split[n - 2] + split[n - 1]);
    }

    public static Path getObjectsPrefixDir(Path objectsDir, Hash hash) {
        return Path.of(objectsDir.toString(), hash.hash().substring(0, 2));
    }
    public static Path getObjectDirByRevision(Path objectsDir, Hash hash) throws GitException {
        if (hash.hash.length() < 5) {
            throw new IllegalArgumentException("Revision would contain 5 or more letters from commit hash");
        }
        Collection<File> files = FileUtils.listFiles(objectsDir.toFile(), null,  true);
        String pattern = Hash.getObjectDir(Path.of(""), hash).toString();
        for (var file : files) {
            if (file.toString().contains(pattern)) {
                return file.toPath();
            }
        }

        throw new GitException(String.format("Revision %s not found", hash));

//        Optional<Path> path = files.stream().filter(file -> file.toString().contains(pattern)).findFirst();
//        if (path.isPresent()) {
//            return path.get();
//        } else {
//            throw new GitException(String.format("Revision %s not found", hash));
//        }
    }
}
