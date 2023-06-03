package ru.itmo.mit.git;

import org.apache.commons.cli.*;
import org.jetbrains.annotations.NotNull;
import ru.itmo.mit.git.commands.*;
import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.entities.GitRepo;
import ru.itmo.mit.git.exceptions.GitFileException;

import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Path;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.stream.Collectors;

import static ru.itmo.mit.git.GitConstants.*;

public class GitCliImpl implements GitCli {
    private final GitRepo repo;
    private PrintStream printStream;

    private Path workingDir;

    public GitCliImpl(String workingDir, PrintStream output) throws GitException, IOException {
        this.workingDir = Path.of(workingDir).normalize().toAbsolutePath();
        this.printStream = output;
        repo = new GitRepo(Path.of(workingDir), true);
    }
    @Override
    public void runCommand(@NotNull String command, @NotNull List<@NotNull String> arguments) throws GitException, GitFileException {
        try {
            Command curCmd = null;

            switch (command) {
                case INIT -> {
                    curCmd = new Init(repo);
                }
                case COMMIT -> {
                    curCmd = new Commit(repo, printStream, arguments);
                }
                case RESET -> {
                    curCmd = new Checkout(repo, arguments, true);
                }
                case LOG -> {
                    curCmd = new Log(repo, printStream, arguments);
                }
                case CHECKOUT -> {
                    curCmd = new Checkout(repo, arguments, false);
                }
                case STATUS -> {
                    curCmd = new Status(repo, printStream);
                }
                case ADD -> {
                    curCmd = new Add(repo, printStream, arguments);
                }
                case RM -> {
                    curCmd = new Rm(repo, arguments);
                }
                default -> {
                    throw new IOException(String.format("Command %s not found", command));
                }
            }
            curCmd.run();

        } catch (IOException e) {
            throw new GitFileException(e.getCause());
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e.getCause());
        }
    }

    @Override
    public void setOutputStream(@NotNull PrintStream outputStream) {
        printStream = outputStream;
    }

    @Override
    public @NotNull String getRelativeRevisionFromHead(int n) throws GitException {
        return String.format("HEAD %d", n);
    }

    public static void main(String[] args) {
        CommandLineParser parser = new DefaultParser();

        Options options = new Options();
        for (var entry : getOptionsWithArgs().entrySet()) {
            String cmd = entry.getKey();
            String description = entry.getValue();
            options.addOption(cmd, true, description);
        }

        for (var entry : getOptionsWithoutArgs().entrySet()) {
            String cmd = entry.getKey();
            String description = entry.getValue();
            options.addOption(cmd, false, description);
        }
        try {
            CommandLine cmd = parser.parse(options, args);
            if (!args[0].equals("git")) {
                throw new ParseException("git command was not found");
            }

            if (cmd.hasOption("h")) {
                help(options);
                return;
            }
            String gitCommand = cmd.getArgs()[1];

//            GitCliImpl cli = new GitCliImpl(System.getProperty("user.dir"), new PrintStream(System.out));
            GitCliImpl cli = new GitCliImpl(Path.of(System.getProperty("user.dir"), "playground").toString(), new PrintStream(System.out));

            cli.runCommand(gitCommand, new ArrayList<>(cmd.getArgList().stream().skip(2).collect(Collectors.toList())));
        } catch (ParseException e) {
            System.err.println("Error parsing command line arguments: " + e.getMessage());
            help(options);
        } catch (GitException | IOException e) {
            System.err.println("Error occured: " + e.getMessage());
        }
    }

    private static void help(Options options) {
        HelpFormatter formatter = new HelpFormatter();
        formatter.printHelp("GitCommandParserExample <command>", options);
    }
    private static Map<String, String> getOptionsWithArgs() {
        Map<String, String> gitCommandMap = new HashMap<>();
        gitCommandMap.put("clone", "Clone a repository into a new directory");
        gitCommandMap.put("add", "Add file contents to the index");
        gitCommandMap.put("commit", "Record changes to the repository");
        gitCommandMap.put("push", "Update remote refs along with associated objects");
        gitCommandMap.put("reset", "Reset current HEAD to the specified state");
        gitCommandMap.put("log", "Show commit logs");
        gitCommandMap.put("rm", "Remove files from the working tree and the index");
        return gitCommandMap;
    }

    private static Map<String, String> getOptionsWithoutArgs() {
        Map<String, String> gitCommandMap = new HashMap<>();
        gitCommandMap.put("init", "Create an empty Git repository");
        gitCommandMap.put("checkout", "Switch branches or restore working tree files");
        gitCommandMap.put("status", "Show the working tree status");
        return gitCommandMap;
    }

}
