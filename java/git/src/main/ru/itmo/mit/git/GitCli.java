package ru.itmo.mit.git;

import org.jetbrains.annotations.NotNull;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.PrintStream;
import java.util.List;

public interface GitCli {
    /*
     * Запустить команду [command] с аргументами [arguments].
     */
    void runCommand(@NotNull String command, @NotNull List<@NotNull String> arguments) throws GitException, GitFileException;

    /*
     * Установить outputStream, в который будет выводиться лог
     */
    void setOutputStream(@NotNull PrintStream outputStream);

    /*
     * Вернуть хеш n-го перед HEAD коммита
     */
    @NotNull String getRelativeRevisionFromHead(int n) throws GitException;
}
