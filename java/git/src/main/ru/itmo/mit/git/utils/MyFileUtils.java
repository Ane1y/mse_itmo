package ru.itmo.mit.git.utils;

import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class MyFileUtils {
    public static List<Path> getAllFiles(Path rootDir) throws GitFileException {
        try (Stream<Path> stream = Files.walk(Paths.get(rootDir.toString()))) {
            return stream.filter(Files::isRegularFile).filter(x -> !x.toString().contains(".git")).collect(Collectors.toList());
        } catch(IOException e) {
            throw new GitFileException("Exception caught while traversing working dir", e.getCause());
        }
    }
    public static void  deleteFiles(Path rootDirectory, Path directoryToExclude) throws IOException {
       Files.walkFileTree(rootDirectory, new SimpleFileVisitor<>() {
           @Override
           public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
               // Delete the file
               if (!file.toString().contains(directoryToExclude.toString())) {
                   Files.delete(file);
               }
               return FileVisitResult.CONTINUE;
           }

           @Override
           public FileVisitResult postVisitDirectory(Path dir, IOException exc) throws IOException {
               // Delete the directory if it's not the one to exclude
               if (!dir.toString().contains(directoryToExclude.toString())) {
                   Files.delete(dir);
               }
               return FileVisitResult.CONTINUE;
           }
       });
    }
}
