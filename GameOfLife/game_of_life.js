const cols = 1000
const rows = 800
const bckg = 155
var colors = [255, 0]
const res=5
var gen = []
var next_gen = []
var grid;

function make2DArray(r, c) {
    let arr = new Array(rows);
    for (let i = 0; i < rows; i++) {
        arr[i] = new Array(cols);
    }
    return arr;
}

function update() {
    var next_grid = make2DArray(rows, cols);
    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            if (i != 0 && j != 0 && i != rows - 1 && j != cols - 1) {
                let sum = grid[i - 1][j] + grid[i - 1][j - 1] + grid[i][j - 1] + grid[i][j + 1] + grid[i + 1][j] + grid[i + 1][j + 1] + grid[i + 1][j - 1] + grid[i - 1][j + 1];
                if (sum == 3) {
                    next_grid[i][j] = 1;
                }
                else if (sum == 2 || sum == 3) {
                    next_grid[i][j] = grid[i][j];
                }
                else {
                    next_grid[i][j] = 0;
                }
            }
        }
    }
    return next_grid;
}


function setup() {
    grid = make2DArray(rows, cols);
    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            // grid[i][j] = floor(random(2));
            grid[i][j] = 0;
        }
    }

    // GLIDER

    // grid[4][5] = 1;
    // grid[4][6] = 1;
    // grid[4][7] = 1;
    // grid[3][7] = 1;
    // grid[2][6] = 1;

    // // OSCILlATOR

    // grid[4][3] = grid[4][5] = grid[4][4] = 1;

    // Glider Gun

    grid[2+5][5+5] = 1;
    grid[3+5][5+5] = 1;
    grid[2+5][6+5] = 1;
    grid[3+5][6+5] = 1;
	

	grid[12+5][5+5] = 1;
	grid[12+5][6+5] = 1;
	grid[12+5][7+5] = 1;
	grid[13+5][4+5] = 1;
	grid[14+5][3+5] = 1;
	grid[15+5][3+5] = 1;
	grid[13+5][8+5] = 1;
	grid[14+5][9+5] = 1;
	grid[15+5][9+5] = 1;
	grid[16+5][6+5] = 1;
	grid[17+5][4+5] = 1;
	grid[17+5][8+5] = 1;
	grid[18+5][5+5] = 1;grid[18+5][6+5] = 1;grid[18+5][7+5] = 1;
	grid[19+5][6+5] = 1;
	grid[22+5][5+5] = 1;grid[23+5][5+5] = 1;grid[22+5][4+5] = 1;grid[23+5][4+5] = 1;grid[22+5][3+5] = 1;grid[23+5][3+5]=1;
	grid[24+5][2+5] = 1;grid[24+5][6+5] = 1;
	grid[26+5][2+5] = 1;grid[26+5][1+5]=1;
	grid[26+5][6+5] = 1;grid[26+5][7+5] = 1;
	grid[36+5][3+5] = 1;grid[37+5][3+5] = 1;grid[36+5][4+5] = 1;grid[37+5][4+5]= 1;

    createCanvas(cols, rows);
}
function draw() {
    background(255);
    for (let x = 0; x < rows; x = x + res) {
        for (let y = 0; y < cols; y = y + res) {
            fill(parseInt(colors[parseInt(grid[x / res][y / res])]));
            stroke(255);
            rect(x, y, res, res);
        }
    }
    grid = update();
}