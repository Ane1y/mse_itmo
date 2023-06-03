package ru.itmo.mit.git.commands;

import ru.itmo.mit.git.entities.GitRepo;

import java.io.IOException;
import java.util.List;


public class Init implements Command {
    private GitRepo repo;

    public Init(GitRepo repo) {
        this.repo = repo;
    }
    @Override
    public void parse(List<String> args) {}

    @Override
    public void run() {

    }

}