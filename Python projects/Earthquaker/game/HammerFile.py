from BackgroundFile import background
from VarsFile import *


class Hammer:
    def __init__(self, window):
        self.window = window
        self.hammer_image = pygame.image.load('img/hammer.bmp').convert()
        self.hammer_image.set_colorkey((255, 255, 255))
        self.hammer_hit_image = pygame.transform.rotate(self.hammer_image, 90)
        self.hammer_image = pygame.transform.rotate(self.hammer_image, 30)
        self.initial_center = (app_width - 64, app_height / 2 - 150)
        self.center = [self.initial_center[0], self.initial_center[1]]
        self.hammer_rect = self.hammer_image.get_rect(center=(int(self.center[0]), int(self.center[1])))
        self.do_hit_the_ground = False
        self.start_animation_part_1 = True
        self.start_animation_part_2 = False
        self.start_animation_part_3 = False
        self.start_animation = False
        self.do_move = True
        self.clock = time.monotonic()
        self.move_parameters = {'x': 100, 'y': 200, 'time': 1}
        self.move_distance = [self.center[0] - self.move_parameters['x'], self.center[1] - self.move_parameters['y']]
        self.ofset = [90, -32]

    def print(self):
        if self.do_hit_the_ground:
            self.window.blit(self.hammer_hit_image, self.hammer_rect)
        else:
            self.window.blit(self.hammer_image, self.hammer_rect)

    def set_move_parameters(self, pos):
        self.move_parameters['x'] = pos[0] + self.ofset[0]
        self.move_parameters['y'] = pos[1] + self.ofset[1]
        self.move_distance = [self.center[0] - self.move_parameters['x'], self.center[1] - self.move_parameters['y']]

    def move(self):
        if self.start_animation:
            if self.start_animation_part_1:
                if self.do_move:
                    self.clock = time.monotonic()
                    self.do_move = False
                if time.monotonic() - self.clock <= self.move_parameters['time']:
                    self.center[0] -= self.move_distance[0] / self.move_parameters['time'] / app_fps
                    self.center[1] -= self.move_distance[1] / self.move_parameters['time'] / app_fps
                    self.hammer_rect = self.hammer_image.get_rect(center=(int(self.center[0]), int(self.center[1])))
                else:
                    pygame.mixer.Channel(0).play(hit_sound)
                    self.center = [self.move_parameters['x'], self.move_parameters['y']]
                    self.hammer_rect = self.hammer_image.get_rect(center=(int(self.center[0]), int(self.center[1])))
                    self.do_move = True
                    self.start_animation_part_1 = False
                    self.start_animation_part_2 = True
                    background.add_crack([self.move_parameters['x'] - self.ofset[0],
                                          self.move_parameters['y'] - self.ofset[1]], random.choice([True, False]))
                    for i in range(10):
                        background.background_crack_specific(background.crack[-1])
                    self.clock = time.monotonic()
            if self.start_animation_part_2:
                if self.do_move:
                    self.clock = time.monotonic()
                    self.do_move = False
                if time.monotonic() - self.clock <= 0.2:
                    self.do_hit_the_ground = True
                else:
                    self.do_move = True
                    self.do_hit_the_ground = False
                    self.start_animation_part_2 = False
                    self.start_animation_part_3 = True
            if self.start_animation_part_3:
                if self.do_move:
                    self.clock = time.monotonic()
                    self.do_move = False
                if time.monotonic() - self.clock <= self.move_parameters['time']:
                    self.center[0] += self.move_distance[0] / self.move_parameters['time'] / app_fps
                    self.center[1] += self.move_distance[1] / self.move_parameters['time'] / app_fps
                    self.hammer_rect = self.hammer_image.get_rect(center=(int(self.center[0]), int(self.center[1])))
                else:
                    self.center = [self.initial_center[0], self.initial_center[1]]
                    self.hammer_rect = self.hammer_image.get_rect(center=(int(self.center[0]), int(self.center[1])))
                    self.do_move = True
                    self.start_animation_part_3 = False
                    self.start_animation_part_1 = True
                    self.start_animation = False


hammer = Hammer(app)
