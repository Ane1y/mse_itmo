package ru.itmo.mit.git.entities;

import ru.itmo.mit.git.exceptions.GitException;
import ru.itmo.mit.git.objects.GitObject;
import ru.itmo.mit.git.utils.Hash;

import java.io.Serializable;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

// а насколько вообще хорошо делать класс с один полем и половину методов
// имплементировать как верни мне пожалуйста все ключи из хеш мапы??
// с одной стороны вроде код становитсяя понятнее, с другой как будто много бесполезного кода
public class Index implements Serializable {
    private final Map<String, Record> index = new HashMap<>(); // key: path + name

    public static String getIndexFilename() {
        return "index";
    }
    public static class Record implements Serializable {
        public Record(GitObject object) {
            this.hash = object.getHash();
            this.type = object.getType();
        }
        public Hash hash;
        public GitObject.Type type;
        public Path path;
    }

    public void add(GitObject object) throws GitException {
        index.put(object.getName(), new Record(object));
    }

    public boolean contains(String filename) {
        return index.containsKey(filename);
    }

    public Record getRecord(String filename) {
        return index.get(filename);
    }

    public List<String> getFiles() {
        return index.keySet().stream().toList();
    }

    public Set<Map.Entry<String, Record>> get() {
        return index.entrySet();
    }

    public void remove(String name) {
        index.remove(name);
    }

}
