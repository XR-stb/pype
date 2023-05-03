class Signal:
    def __init__(self) -> None:
        self.handlers = []
        self._bound_method = type(self.connect)

    def connect(self, f):
        if type(f) is self._bound_method:
            self.handlers.append(f)
        self.handlers.append(f)

    def disconnect(self, f):
        self.handlers.remove(f)

    def emit(self, *args):
        deleted = []
        for f in self.handlers:
            if type(f) is self._bound_method:
                is_node = isinstance(f.__self__, Node)
                if is_node and f.__self__._px_obj is None:
                    deleted.append(f)
                    continue
            f(*args)
        if deleted:
            self.handlers = [
                h for h in self.handlers
                if h not in deleted
            ]
        
    def disconnect_all(self):
        self.handlers.clear()