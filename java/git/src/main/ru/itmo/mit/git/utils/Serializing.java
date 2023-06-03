package ru.itmo.mit.git.utils;

import ru.itmo.mit.git.entities.Index;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.*;
import java.nio.file.Path;

public class Serializing {
    public static<C> C readClass(Path path) throws GitFileException {
        try (FileInputStream serializedFile = new FileInputStream(path.toString());
             ObjectInputStream in = new ObjectInputStream(serializedFile)) {
            return (C)in.readObject();
        } catch (IOException i) {
            throw new GitFileException(String.format("Error occured while serealizing file at %s",path));
        } catch (ClassNotFoundException c) {
            throw new RuntimeException("Class not found");
        }
    }
    public static void writeClass(Object obj, Path path) throws GitFileException {
        try (var fileOut = new FileOutputStream(path.toString());
             ObjectOutputStream out = new ObjectOutputStream(fileOut);){
            out.writeObject(obj);
        } catch (IOException e) {
            throw new GitFileException("Error occurred when index was deserializing");
        }
    }
}
