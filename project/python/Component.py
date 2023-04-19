class Component:
    Awake = None
    Update = None
    OnDestroy = None

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

    