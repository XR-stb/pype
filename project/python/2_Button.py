class Button(Node):
    def __init__(self):
        self.clicked = Signal()
        super(Button, self).__init__()

    def on_click(self):
        self.clicked.emit()