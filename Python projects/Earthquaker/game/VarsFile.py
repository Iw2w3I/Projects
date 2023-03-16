import random
import time
import pygame
import pygame.freetype


app_width = 1200
app_height = 800
app_fps = 60

pygame.init()
app = pygame.display.set_mode((app_width, app_height))
pygame.display.set_caption("Earthquaker")

font1 = pygame.freetype.SysFont('arial', 40)
font2 = pygame.freetype.SysFont('arial', 24)
font3 = pygame.freetype.SysFont('arial', 18)
font4 = pygame.freetype.SysFont('arial', 24, 1, 1)
font5 = pygame.freetype.SysFont('arial', 32, 0, 1)


def increase_cost_formula(cost):
    new_cost = int(cost * 1.1)
    new_cost -= new_cost % 10
    return new_cost


pygame.mixer.set_num_channels(1)
hit_sound = pygame.mixer.Sound('sound/Hit.wav')
pygame.mixer.music.load('sound/Theme1.mp3')
pygame.mixer.music.set_volume(0.2)
pygame.mixer.music.play(-1)

decorations = []
