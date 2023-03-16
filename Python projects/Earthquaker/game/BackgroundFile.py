from VarsFile import *
from PrinterFile import printer


class Crack:
    '''
    preferred_side variants:
        - m = mid
        - l = left
        - r = right
        - ll = far left
        - rr = far right
    '''
    size = 3
    x = app_width // 2
    y = app_height // 2 - 1
    start_x = x
    start_y = y
    preferred_side = 'm'
    add_new_cracks = True

    def __init__(self, preferred_side, *args_position):
        self.preferred_side = preferred_side
        if args_position:
            self.x = args_position[0]
            self.y = args_position[1]

    def set_pos(self, pos):
        x = pos[0]
        y = pos[1]


class Background:
    def __init__(self, window):
        self.window = window
        self.background = pygame.Surface((app_width, app_height))
        self.background.fill((255, 255, 255))
        pygame.draw.rect(self.background, (0, 0, 0), (0, app_height // 2, app_width, app_height))
        self.crack = [Crack('r'), Crack('ll')]
        self.crack_cost = 300
        self.count_white = 0
        self.count_black = app_width * app_height // 2
        self.crack_percent = 0
        self.amount_of_trees = random.randint(8, 12)
        self.amount_of_cars = random.randint(3, 7)
        decoration_spots = []
        for i in range(20, app_width - 20):
            decoration_spots.append(i)
        for i in range(self.amount_of_trees):
            spot = random.choice(decoration_spots)
            decoration_spots.remove(spot)
            for delta in range(1, 40):
                if decoration_spots.__contains__(spot - delta):
                    decoration_spots.remove(spot - delta)
                if decoration_spots.__contains__(spot + delta):
                    decoration_spots.remove(spot + delta)
            decorations.append(DecorationTree(window, spot))
        for i in range(self.amount_of_cars):
            spot = random.choice(decoration_spots)
            decoration_spots.remove(spot)
            for delta in range(1, 40):
                if decoration_spots.__contains__(spot - delta):
                    decoration_spots.remove(spot - delta)
                if decoration_spots.__contains__(spot + delta):
                    decoration_spots.remove(spot + delta)
            decorations.append(DecorationCar(window, spot))

    def print(self):
        self.window.blit(self.background, (0, 0))

    def background_crack(self, increment):
        self.new_cracks_condition()
        for i in range(increment):
            c = random.choice(self.crack)
            self.background_crack_specific(c)

    def background_crack_specific(self, crack):
        self.new_cracks_condition()
        crack.y += 1
        crack.x += random.randint(-2, 2)
        if crack.preferred_side == 'l':
            crack.x -= random.randint(0, 1)
        if crack.preferred_side == 'r':
            crack.x += random.randint(0, 1)
        if crack.preferred_side == 'll':
            crack.x -= random.randint(0, 2)
        if crack.preferred_side == 'rr':
            crack.x += random.randint(0, 2)
        pygame.draw.rect(self.background, (255, 255, 255), (crack.x, crack.y, crack.size, crack.size))
        if crack.y < app_height:
            self.count_white += crack.size ** 2
            self.count_black -= crack.size ** 2

    def new_cracks_condition(self):
        for c in self.crack:
            if c.y > app_height * 0.7 and c.add_new_cracks:
                self.add_crack([c.x, c.y], False, c.preferred_side)
                c.add_new_cracks = False

    def add_crack(self, pos, add_new_cracks, *args_parent_preferred_side):
        '''
            format of inputs
                - pos: [x, y]
                - add_new_cracks: True/False
                - *args_parent_preferred_side: ['m', 'l', 'r', 'll', 'rr']
        '''
        choice = ['m', 'l', 'r', 'll', 'rr']
        for i in args_parent_preferred_side:
            choice.remove(i)
        self.crack.append(Crack('{0}'.format(random.choice(choice)), pos[0], pos[1]))
        self.crack[-1].add_new_cracks = add_new_cracks

    def find_crack_percent(self):
        self.crack_percent = self.count_white / self.count_black
        printer.print_text([app_width // 2, 50], font5, '{:.2f}% destruction'.format(100 * self.crack_percent), [], 0)


class DecorationTree:
    def __init__(self, window, pos_x):
        self.window = window
        self.tree_image = pygame.image.load('img/tree.bmp').convert()
        self.tree_image.set_colorkey((255, 255, 255))
        coef = random.choice([1, 1.2])
        self.tree_image = pygame.transform.scale(self.tree_image, (int(32 / coef), int(32 / coef)))
        self.center = [pos_x, app_height // 2 - int(16 / coef)]
        self.tree_rect = self.tree_image.get_rect(center=(int(self.center[0]), int(self.center[1])))

    def print(self):
        self.window.blit(self.tree_image, self.tree_rect)


class DecorationCar:
    def __init__(self, window, pos_x):
        self.window = window
        self.center = [pos_x, app_height // 2 - 16]
        self.car_image = pygame.image.load('img/car.bmp').convert()
        self.car_image.set_colorkey((255, 255, 255))
        if random.randint(0, 1):
            self.car_image = pygame.transform.flip(self.car_image, 1, 0)
        self.car_rect = self.car_image.get_rect(center=(int(self.center[0]), int(self.center[1])))

    def print(self):
        self.window.blit(self.car_image, self.car_rect)


background = Background(app)
