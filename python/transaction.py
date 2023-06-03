class Storage:
    def __init__(self):
        self.content_manager = ContentManager()

    def edit(self):
        return self.content_manager

    def __getitem__(self, item):
        return self.content_manager[item]


class ContentManager:
    def __init__(self):
        self._data = {}
        self._temp = {}

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if not exc_tb:
            self._data.update(self._temp)

    def __getitem__(self, item):
        return self._temp[item]

    def __setitem__(self, key, value):
        self._temp[key] = value
