from pype import *

sp = Sprite2D()
sp.scale = 0.25
sp.position = Vector2(100, 100)
sp.texture = load("assets/fox.png")

# tween = sp.to("position", Vector2(300, 100), 2)

t = TweenList(sp, [
    Tweener(sp, "position", Vector2(300, 100), 1.5),
    Tweener(sp, "position", Vector2(300, 200), 1.5),
    Tweener(sp, "scale", sp.scale*3, 2),
])

t.play()