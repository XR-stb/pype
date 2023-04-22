class Signal:
    def __init__(self) -> None:
        self.handlers = []
        self.tp_bound_method = type([].append)

    def connect(self, f):
        if type(f) is self.tp_bound_method:
            self.handlers.append(f)
            # 以bound method形式绑定时，__self__必须是Node的实例
            assert isinstance(f.__self__, Node)
            assert f.__self__._px_obj is not None
        self.handlers.append(f)

    def disconnect(self, f):
        self.handlers.remove(f)

    def emit(self, *args):
        deleted = []
        for f in self.handlers:
            if type(f) is self.tp_bound_method:
                # 检查Node实例是否存活
                if f.__self__._px_obj is None:
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