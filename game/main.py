from pype import *

sp = Sprite2D()
sp.scale = 0.5
sp.position = Vector2(400, 300)

anim = FrameAnimation()
anim.frames = load_dir("assets/fox/run")
anim.speed = 12
anim.loop = True

animator = FrameAnimator()
animator['run'] = anim
animator.parent = sp
animator.play('run')