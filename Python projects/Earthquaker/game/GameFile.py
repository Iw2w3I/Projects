import TextFile
from VarsFile import *
from PrinterFile import printer
from BackgroundFile import background
from HammerFile import hammer
from EarthquakeFile import earthquake


class Game:
    def __init__(self, window):
        self.window = window
        self.score = 0
        self.click_increment = 1
        self.constant_increment = 0
        self.start_bonus_clock = True
        self.fast_click_bonus = 15 * self.click_increment
        self.clock1 = time.monotonic()
        self.text_score = TextFile.Text(self.window, font1, '0', [], 0)
        self.text_cps = TextFile.Text(self.window, font1, 'cps: 0', [], 0)
        self.cps = 0
        self.do_cps_check = True

    def check_event(self, event):
        if self.score >= background.crack_cost:
            self.buy_a_crack(event)
        self.buy_an_earthquake(event)
        if (event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE) \
                or event.type == pygame.MOUSEBUTTONDOWN:
            self.score += self.click_increment
            self.cps += 1
            background.background_crack(1)

    def cps_check(self):
        if self.do_cps_check:
            self.do_cps_check = False
            self.cps = 0
            self.clock1 = time.monotonic()
        if time.monotonic() - self.clock1 >= 1:
            if self.cps >= 7:
                self.score += self.fast_click_bonus
                self.fast_click_bonus = 15 * self.click_increment
                background.background_crack(self.fast_click_bonus // 3)
                printer.print_text_speed_click_bonus(self)
            self.do_cps_check = True
            printer.print_cps(self)
            self.score += self.constant_increment
            background.background_crack(self.constant_increment)

    def buy_a_crack(self, event):
        if not hammer.start_animation and (event.type == pygame.MOUSEBUTTONUP and
                                           hammer.hammer_rect.collidepoint(pygame.mouse.get_pos())):
            self.score -= background.crack_cost
            printer.print_buying(background.crack_cost)
            background.crack_cost = increase_cost_formula(background.crack_cost)
            hammer.start_animation = True
            hammer.set_move_parameters([random.randint(30, app_width - 30), app_height // 2])

    def buy_an_earthquake(self, event):
        for i in range(len(earthquake.buttons)):
            if earthquake.buttons[i].rect.collidepoint(pygame.mouse.get_pos()) and \
                    event.type == pygame.MOUSEBUTTONUP and self.score >= earthquake.buttons[i].cost:
                if i == 0:
                    earthquake.gives_points = 1
                if i == 1:
                    earthquake.gives_points = 5
                    earthquake.click_increment = 1
                if i == 2:
                    earthquake.gives_points = 10
                    earthquake.click_increment = 3
                self.score -= earthquake.buttons[i].cost
                printer.print_buying(earthquake.buttons[i].cost)
                earthquake.buttons[i].cost = increase_cost_formula(earthquake.buttons[i].cost)
                self.constant_increment += earthquake.gives_points
                self.click_increment += earthquake.click_increment


game = Game(app)
