from VarsFile import *
from PrinterFile import printer
from BackgroundFile import background
from HammerFile import hammer
from GameFile import game
from EarthquakeFile import earthquake


game_clock = pygame.time.Clock()
pause = False

while True:
    game_clock.tick(app_fps)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()
        elif not pause:
            game.check_event(event)

    if not pause:
        background.print()
        game.cps_check()
        hammer.move()
        background.find_crack_percent()
    if background.crack_percent >= 1:
        background.crack_percent = 1
        background.print()
        printer.print_text([app_width // 2, 100], font1, "You won", [], 0)
        pause = True
    printer.print_decoration(decorations)
    printer.print_score(game)
    printer.print_hit(background)
    printer.print_earthquake(earthquake)
    printer.print_hammer(hammer)
    printer.print_all()
    pygame.display.flip()

