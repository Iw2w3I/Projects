import TextFile
from VarsFile import *
from PrinterFile import printer


class Button:
    def __init__(self, window, pos, cost):
        '''
        format of inputs
            - window: graphic window
            - pos: [x, y]
            - cost: int()
        '''
        self.window = window
        self.cost = cost
        self.position = pos
        self.text = TextFile.Text(self.window, font1, 'button', [], 0)
        self.rect = self.text.text_rect.inflate(30, 10)

    def print(self):
        self.text.set_text('for {0}'.format(self.cost))
        self.text.set_position(self.position[0], self.position[1])
        self.rect = self.text.text_rect.inflate(30, 10)
        printer.print_text(self.text.position, font1, self.text.text, [], 0)
        pygame.draw.rect(self.window, (0, 0, 0), self.rect, 3)


class Earthquake:
    def __init__(self, window):
        self.window = window
        self.buttons = [Button(window, [150, 130], 70), Button(window, [150, 230], 500),
                        Button(window, [150, 330], 2000)]
        self.gives_points = 1
        self.click_increment = 0
        self.description = TextFile.Text(self.window, font3, '', [], 0)

    def print(self):
        for i in range(len(self.buttons)):
            self.buttons[i].print()
        for i in range(len(self.buttons)):
            if self.buttons[i].rect.collidepoint(pygame.mouse.get_pos()):
                if i == 0:
                    self.gives_points = 1
                    self.click_increment = 0
                if i == 1:
                    self.gives_points = 5
                    self.click_increment = 1
                if i == 2:
                    self.gives_points = 10
                    self.click_increment = 3
                printer.print_text([pygame.mouse.get_pos()[0] + 50, pygame.mouse.get_pos()[1] - 50], font3,
                                        'gives +{0} points per second and +{1} per click'
                                        .format(self.gives_points, self.click_increment), [], 0)


earthquake = Earthquake(app)
