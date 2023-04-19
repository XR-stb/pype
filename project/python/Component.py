class Component:
    Awake = None
    Update = None
    OnDestroy = None

    # 内部钩子
    _update_coroutines = None

    def __init__(self, gameObject):
        self.gameObject = gameObject

    def GetComponent(self, tp):
        return self.gameObject.GetComponent(tp)

    def GetComponentInChildren(self, tp):
        return self.gameObject.GetComponentInChildren(tp)

class PainterBehaviour(Component):
    def __init__(self, gameObject):
        super(PainterBehaviour, self).__init__(gameObject)
        self.coroutines = []

    def StartCoroutine(self, coroutine):
        self.coroutines.append(coroutine)

    def StopCoroutine(self, coroutine):
        self.coroutines.remove(coroutine)

    def StopAllCoroutines(self):
        self.coroutines.clear()

    def _update_coroutines(self):
        for coroutine in self.coroutines:
            obj = next(coroutine)
            if obj is StopIteration:
                self.coroutines.remove(coroutine)


def WaitForEndOfFrame():
    yield None

def WaitForSeconds(seconds):
    total = 0
    while total < seconds:
        total += Time.deltaTime
        yield None

    