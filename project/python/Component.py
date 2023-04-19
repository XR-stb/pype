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
    pass