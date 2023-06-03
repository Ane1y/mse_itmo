package ru.itmo.mit.git.exceptions;


public class GitFileException extends GitException {

    public GitFileException() {
    }

    public GitFileException(String message, Throwable cause) {
        super(message, cause);
    }

    public GitFileException(Throwable cause) {
        super(cause);
    }

    public GitFileException(String oldFile, String newFile, Throwable cause) {
        super(String.format("Error occured while processing file %s to %s", oldFile, newFile), cause);
    }
    public GitFileException(String file) {
        super(String.format("No file found on %s", file));
    }

    public GitFileException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
