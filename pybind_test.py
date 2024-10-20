from gz_biped_gym import BlackbirdEnv
import numpy as np

env = BlackbirdEnv.BlackbirdGazebo(render_mode="human")

i = 0
while (i < 1e6):
    action = np.random.uniform(low=-50.0, high=50.0, size=(10,))
    state, reward, terminal, _ = env.step(action)
    if (terminal):
        print("reached a terminal. resetting...")
        env.reset()
    i += 1