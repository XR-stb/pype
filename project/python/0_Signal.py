class Signal:
    def __init__(self) -> None:
        self.handlers = []

    def connect(self, f):
        self.handlers.append((f.__self__, f.__func__))

    def disconnect(self, f):
        self.handlers.remove(f)

    def emit(self, *args):
        deleted = []
        for obj, func in self.handlers:
            if obj._px_obj is None:
                deleted.append(obj)
                continue
            func(obj, *args)
        if deleted:
            self.handlers = [
                h for h in self.handlers
                if h not in deleted
            ]
        
    def disconnect_all(self):
        self.handlers.clear()