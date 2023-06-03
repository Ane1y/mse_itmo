package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.util.List;

public interface Command {

    void parse(List<String> args) throws GitFileException, GitException;

    void run() throws GitException, GitFileException;
}
