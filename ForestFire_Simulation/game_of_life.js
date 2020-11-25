const cols = 1000
const rows = 800
const bckg = 155
var colors = [255, 180, 100, 0]
const res = 5
var gen = []
var gen_time;
var next_gen = []
var grid;

var code = [0, 0.25, 1, 0];


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
                if (grid[i][j] == 3) {
                    next_grid[i][j] = 3;    // burnt  
                    continue;
                }
                else if (grid[i][j] == 2) {
                    console.log(gen_time[i][j]);
                    gen_time[i][j]--;
                    if (gen_time[i][j] == 0) {
                        next_grid[i][j] = 3;    // burnt
                    }
                    else {
                        next_grid[i][j] = 2;    // burning
                    }
                }
                else {
                    let sum = code[grid[i - 1][j]] + code[grid[i - 1][j - 1]] + code[grid[i][j - 1]] + code[grid[i][j + 1]] + code[grid[i + 1][j]] + code[grid[i + 1][j + 1]] + code[grid[i + 1][j - 1]] + code[grid[i - 1][j + 1]];
                    // // UPDATION RULES HERE

                    // if (sum == 4) {
                    //     next_grid[i][j] = 1;
                    // }
                    // else if (sum == 3) {
                    //     next_grid[i][j]] = code[grid[i][j];
                    // }
                    // else {
                    //     next_grid[i][j] = 0;
                    // }

                    sum = sum / 5.0 + (code[grid[i][j]]) / 4.0;
                    if (sum >= 0.5) {
                        next_grid[i][j] = 2;
                    }
                    else if (sum >= 0.25 || grid[i][j] == 1) {
                        next_grid[i][j] = 1;
                    }
                    else {
                        next_grid[i][j] = 0;
                    }
                }
            }
        }
    }
    return next_grid;
}


function setup() {
    grid = make2DArray(rows, cols);
    gen_time = make2DArray(rows, cols);
    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            grid[i][j] = floor(random(100));
            if (grid[i][j] >= 80) {
                grid[i][j] = 3;
            }
            else {
                grid[i][j] = 0;
            }
            gen_time[i][j] = 3;
        }
    }
    // for (let i = 0; i < 30;` i++) {
    //     for (let j = 0; j < 30; j++) {
    //         grid[i][j] = floor(random(2))*2;
    //     }
    // }`
    // GLIDER

    grid[4][5] = 2;
    grid[4][6] = 2;
    grid[4][7] = 2;
    grid[3][7] = 2;
    grid[2][6] = 2;

    // // OSCILlATOR

    grid[4][3] = grid[4][5] = grid[4][4] = 2;

    // Glider Gun

    grid[2 + 5][5 + 5] = 2;
    grid[3 + 5][5 + 5] = 2;
    grid[2 + 5][6 + 5] = 2;
    grid[3 + 5][6 + 5] = 2;


    // grid[12 + 5][5 + 5] = 2;
    // grid[12 + 5][6 + 5] = 2;
    // grid[12 + 5][7 + 5] = 2;
    // grid[13 + 5][4 + 5] = 2;
    // grid[14 + 5][3 + 5] = 2;
    // grid[15 + 5][3 + 5] = 2;
    // grid[13 + 5][8 + 5] = 2;
    // grid[14 + 5][9 + 5] = 2;
    // grid[15 + 5][9 + 5] = 2;
    // grid[16 + 5][6 + 5] = 2;
    // grid[17 + 5][4 + 5] = 2;
    // grid[17 + 5][8 + 5] = 2;
    // grid[18 + 5][5 + 5] = 2; grid[18 + 5][6 + 5] = 2; grid[18 + 5][7 + 5] = 2;
    // grid[19 + 5][6 + 5] = 2;
    // grid[22 + 5][5 + 5] = 2; grid[23 + 5][5 + 5] = 2; grid[22 + 5][4 + 5] = 2; grid[23 + 5][4 + 5] = 2; grid[22 + 5][3 + 5] = 2; grid[23 + 5][3 + 5] = 2;
    // grid[24 + 5][2 + 5] = 2; grid[24 + 5][6 + 5] = 2;
    // grid[26 + 5][2 + 5] = 2; grid[26 + 5][1 + 5] = 2;
    // grid[26 + 5][6 + 5] = 2; grid[26 + 5][7 + 5] = 2;
    // grid[36 + 5][3 + 5] = 2; grid[37 + 5][3 + 5] = 2; grid[36 + 5][4 + 5] = 2; grid[37 + 5][4 + 5] = 2;

    createCanvas(cols, rows);
}
function draw() {
    background(255);
    for (let x = 0; x < rows; x = x + res) {
        for (let y = 0; y < cols; y = y + res) {
            if(grid[x/res][y/res] == 0)
            {
                fill(0,255,0);
            }
            else if(grid[x/res][y/res] == 1)
            {
                fill(255,255,0);
            }
            else if(grid[x/res][y/res] == 2)
            {
                fill(255,0,0);
            }
            else if(grid[x/res][y/res] == 3)
            {
                fill(100,150,0);
            }
            // nostroke();
            // fill(parseInt(colors[parseInt(grid[x / res][y / res])]));
            // stroke(255);
            rect(x, y, res, res);
        }
    }
    grid = update();
}