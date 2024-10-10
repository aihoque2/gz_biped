import gym
import numpy as np
from gym import spaces
import sys
import blackbird_rl

class BlackbirdGazebo(gym.Env):
    metadata = {"render_modes": ["human", "rgb_array"], "render_fps": 4}

    def __init__(self, render_mode=None):
        sys.path.append("../../build")
        set_gui = False
        if render_mode is not None:
            set_gui = True
        else:
            set_gui = False
        self.sim = blackbird_rl.TrainSimulator(gui=set_gui)

        obs_low = np.full(32, -np.inf)  # -inf for each element
        obs_high = np.full(32, np.inf)  # inf for each element

        self.observation_space = spaces.Box(low=obs_low, high=obs_high, dtype=np.float64)

        axn_low = np.full(10, -20.0)
        axn_hi = np.full(10, 20.0)
        self.action_space = spaces.Box(low=axn_low, high=axn_hi, dtype=np.float64)
        self.steps = 0
        
        self.Y_WEIGHT = -100.0

        self.POWER_WEIGHT = 0.005
    
    def det_terminal(self):
        """
        determine terminal
        """

        if (self.steps >= 10000):
            return True
        if (self.sim.is_terminal()):
            return True
        return False

    def reset(self):
        self.sim.reset_sim()
        self.steps = 0

    def step(self, action):
        
        self.sim.step(action)
        state = self.sim.get_state()

        pose_y = state[2]
        
        power = 0.0
        for i in range(len(action)):
            power += action[i] * state[i+12]

        reward = self.Y_WEIGHT*pose_y - self.POWER_WEIGHT*power

        self.steps += 1
        return np.array(state, dtype=np.float32), reward, self.det_terminal(), False, {}
