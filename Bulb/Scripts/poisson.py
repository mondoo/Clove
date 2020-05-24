#Generates an array of points for a poisson disk sampler and prints them to a file

from math import cos, sin, floor, sqrt, pi, ceil
from random import random

class Point:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

def distance(point_a, point_b):
    dx = point_a.x - point_b.x
    dy = point_b.y - point_b.y
    return sqrt(dx * dx + dy * dy)

def generate_poisson(width, height, min_dist, new_points_count):
    cell_size = min_dist / sqrt(2)

    grid_width = int(ceil(width / cell_size))
    grid_height = int(ceil(height / cell_size))

    grid = [None] * (grid_width * grid_height)

    def grid_coords(point: Point):
        return int(floor(point.x / cell_size)), int(floor(point.y / cell_size))

    def fits(point: Point, grid_x, grid_y):
        y_range = list(range(max(grid_y - 2, 0), min(grid_y + 3, grid_height)))
        x_range = list(range(max(grid_x - 2, 0), min(grid_x + 3, grid_width)))
        for x in x_range:
            for y in y_range:
                grid_point = grid[x + y * grid_width]
                if grid_point is None:
                    continue
                if distance(point, grid_point) <= min_dist:
                    return False
        return True
    
    first_point = Point(width * random(), height * random())

    queue = [first_point]
    grid_x, grid_y = grid_coords(first_point)
    grid[grid_x + grid_y * grid_width] = first_point

    while queue:
        queue_index = int(random() * len(queue)) #selects a random element from the queue
        point = queue[queue_index]

        queue[queue_index] = queue[-1]
        queue.pop()

        for _ in range(new_points_count):
            r1 = random()
            r2 = random()

            radius = min_dist * (r1 + 1)
            angle = 2 * pi * r2

            x = point.x * radius * cos(angle)
            y = point.y * radius * sin(angle)
            if not (0 <= x < width and 0 <= y < height):
                continue

            new_point = Point(x, y)
            grid_x, grid_y = grid_coords(new_point)
            if not fits(new_point, grid_x, grid_y):
                continue

            queue.append(new_point)
            grid[grid_x + grid_y * grid_width] = new_point

    return [point for point in grid if point is not None]

def main():
    points = generate_poisson(5, 5, 2.0, 0)

    for point in points:
        print("X: " + str(point.x) + " Y: " + str(point.y))

if __name__ == "__main__":
    main()