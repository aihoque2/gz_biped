import gym
import numpy as np
from gym import spaces

import blackbird_rl

class BlackbirdGazebo(gym.Env):
    metadata = {"render_modes": ["human", "rgb_array"], "render_fps": 4}

    def __init__(self):
        self.sim = blackbird_rl.TrainSimulator(gui=True)
        self.observation_space = spaces.Dict(
            {
                "agent": spaces.Box(0, size - 1, shape=(25,), dtype=int),
                "target": spaces.Box(0, size - 1, shape=(3,), dtype=int),
            }
        )

        self.action_space = np.array((10,))
        self.steps = 0

    
    def det_terminal(self):
        if (self.steps >= 10000):
            return True
        if (self.sim.is_terminal()):
            return True
        return False

    def reset(self):
        self.sim.reset_sim()


    def step(self, action):
        
        pass
