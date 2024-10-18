from gz_biped_gym import BlackbirdEnv
import numpy as np

env = BlackbirdEnv.BlackbirdGazebo(render_mode="human")

while(True):
    action = np.random.uniform(low=-30.0, high=30.0, size=(10,))
