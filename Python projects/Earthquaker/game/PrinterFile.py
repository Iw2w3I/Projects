import TextFile
from VarsFile import *


class Printer:
    def __init__(self, window):
        self.window = window
        self.print_queue = []

    def print_all(self):
        '''
        self.print_queue contains elements
            - object with function .print()
            - time_of_creation
            - time_of_existence
        '''
        index_to_pop = []
        for i in range(len(self.print_queue)):
            self.print_queue[i][0].print()
            if time.monotonic() - self.print_queue[i][1] >= self.print_queue[i][2]:
                index_to_pop.insert(0, i)
        for i in index_to_pop:
            self.print_queue.pop(i)

    def print_text(self, pos, font, text, tag, time_of_existence):
        '''
        format of inputs
            - pos: [x, y]
            - font: fontx
            - text: str()
            - tag: [...]
            - time_of_existence: seconds
        '''
        new_text = TextFile.Text(self.window, font, text, tag, time_of_existence)
        new_text.set_position(pos[0], pos[1])
        self.print_queue.append([new_text, time.monotonic(), time_of_existence])

    def print_score(self, game):
        self.print_text([app_width // 2, app_height // 3], font1, '{0}'.format(game.score), [], 0)

    def print_text_speed_click_bonus(self, game):
        self.print_text([app_width // 2, app_height // 3 - 50], font2,
                        'fast click bonus: +{0}'.format(game.fast_click_bonus), ['fading 100', 'moving 0 -100'], 2)

    def print_cps(self, game):
        self.print_text([app_width // 2 + 280, 50], font4, 'cps: {0}'.format(game.cps), [], 1)

    def print_buying(self, cost):
        self.print_text([app_width // 2 - 150, app_height // 3], font2,
                        'sold: -{0}'.format(cost), ['fading 100', 'moving -100 -100'], 2)

    def print_hit(self, background):
        self.print_text([app_width - 110, 50], font4, 'crack the ground', [], 0)
        self.print_text([app_width - 110, 100], font1, 'for {0}'.format(background.crack_cost), [], 0)

    def print_hammer(self, hammer):
        self.print_queue.append([hammer, time.monotonic(), 0])

    def print_decoration(self, decorations):
        for i in decorations:
            self.print_queue.append([i, time.monotonic(), 0])

    def print_earthquake(self, earthquake):
        self.print_text([150, 50], font4, 'summon an earthquake', [], 0)
        self.print_queue.append([earthquake, time.monotonic(), 0])


printer = Printer(app)
