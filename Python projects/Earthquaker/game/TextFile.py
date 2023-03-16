from VarsFile import *


class Text:
    def __init__(self, window, font, text, type=[], print_time=0, pos_x=0, pos_y=0):
        '''
        :param window: pygame.display
        :param text: any str(...) object
        :param type: tuple of str
        :param print_time: seconds
        :param pos_x: int
        :param pos_y: int

        possible types:
            - 'fading %'
            - 'moving x y'
        '''
        self.window = window
        self.color1 = pygame.Color(0, 0, 0, 255)
        self.font = font
        self.text = str(text)
        self.text_surface, self.text_rect = self.font.render(self.text, self.color1)
        self.position = self.text_rect.center = [pos_x, pos_y]
        self.do_print = False
        self.clock1 = time.monotonic()
        self.print_time = 0
        self.fading = 0
        self.do_fading = False
        self.do_moving = False
        for i in type:
            if i.split(' ')[0] == 'fading':
                self.do_fading = True
                self.fading = int(i.split(' ')[1])
            elif i.split(' ')[0] == 'moving':
                self.do_moving = True
                self.moving = [int(i.split(' ')[1]), int(i.split(' ')[2])]
        self.set_print_time(print_time)

    def set_text(self, text):
        self.text = str(text)
        self.text_surface, self.text_rect = self.font.render(self.text, self.color1)

    def set_position(self, pos_x, pos_y):
        self.position = [pos_x, pos_y]
        self.text_rect.center = (int(self.position[0]), int(self.position[1]))

    def get_width(self):
        return self.text_rect.width

    def get_height(self):
        return self.text_rect.height

    def set_print_time(self, time):
        if time <= 0:
            self.do_print = True
            if self.do_fading:
                self.fading = 255 / app_fps
        else:
            self.print_time = time
            if self.do_fading:
                self.fading = 255 * self.fading / 100 / time / app_fps
            if self.do_moving:
                self.moving = [self.moving[0] / time / app_fps, self.moving[1] / time / app_fps]

    def print(self):
        if self.do_print or time.monotonic() - self.clock1 <= self.print_time:
            if self.do_fading and self.color1.a >= self.fading:
                self.color1.a = int(self.color1.a - self.fading)
            if self.do_moving:
                self.position = (self.position[0] + self.moving[0],
                                 self.position[1] + self.moving[1])
                self.text_rect.center = (int(self.position[0]), int(self.position[1]))
            self.text_surface = self.font.render(self.text, self.color1)[0]
            self.window.blit(self.text_surface, self.text_rect)
