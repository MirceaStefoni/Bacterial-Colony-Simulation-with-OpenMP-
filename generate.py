import random

def generate_grid(rows, cols):
    grid = []
    for _ in range(rows):
        # Generate a single row with random placement of 'X' and '.'
        row = ''.join(random.choice(['X', '.']) for _ in range(cols))
        grid.append(row)
    return grid

def save_grid_to_file(grid, filename):
    with open(filename, 'w') as file:
        # Write the size of the grid as the first line
        file.write(f"{len(grid)} {len(grid[0])}\n")
        # Write each row of the grid
        for row in grid:
            file.write(row + "\n")

def main():
    rows, cols = 500, 500
    filename = "bacteria500.txt"
    
    # Generate the grid
    grid = generate_grid(rows, cols)
    
    # Save the grid to a file
    save_grid_to_file(grid, filename)
    print(f"Grid of size {rows}x{cols} saved to {filename}")

main()