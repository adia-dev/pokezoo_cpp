const fs = require('fs');

// Check if the file path is provided as a command-line argument
if (process.argv.length < 3) {
    console.log('Usage: node create_pokemons.js <file_path>');
    process.exit(1);
}

const file_path = process.argv[2];

// Constants
const DEFAULT_WIDTH = 32;
const DEFAULT_HEIGHT = 32;
const SPRITESHEET_WIDTH = 1024;
const SPRITESHEET_HEIGHT = 5408;
const SKIPS = [
    {
        from: { x: 832, y: 1536 },
        to: { x: 512, y: 1760 }
    }
];
const POKEMON_WITH_GENDERS_IDS = [3, 25, 154, 202, 208, 214, 415, 443, 444, 445, 449, 450, 487];
const BIG_POKEMONS = ["Steelix", "Lugia", "Ho-oh", "Wailord", "Groudon", "Kyogre", "Rayquaza", "Dialga", "Palkia", "Regigigas", "Giratina", "Arceus"];

// Read the file content and process each line
const file_content = fs.readFileSync(file_path, 'utf8');
const pokemons = [];
let x = 0;
let y = 0;
let big_pokemon_in_row = false;


file_content.split('\n').forEach(process_pokemon_line);

// Create the JSON object
const json = {
    texture_name: "bw_old_overworld.png",
    pokemons
};

// Write the JSON object to a file
fs.writeFileSync('src/assets/json/bw_old_overworld.json', JSON.stringify(json, null, 2));

/**
 * Process a line of the input file and convert it into a Pokemon object.
 * @param {string} line - The line containing Pokemon data..
 */
function process_pokemon_line(line) {
    const [id, name, type1, type2, hp, attack, defense, spAttack, spDefense, speed] = line.split('\t');
    const pokemon_data = { id, name, type1, type2, hp, attack, defense, spAttack, spDefense, speed, gender: 1 };
    const is_big_pokemon = BIG_POKEMONS.includes(pokemon_data.name);

    if (is_big_pokemon) {
        if (x + DEFAULT_WIDTH * 2 >= SPRITESHEET_WIDTH) {
            x = 0;
            y += DEFAULT_HEIGHT * (big_pokemon_in_row ? 6 : 4);
        }
        big_pokemon_in_row = true;
    }

    const pokemon = format_pokemon(pokemon_data, x, y, is_big_pokemon);

    pokemons.push(pokemon);

    if (POKEMON_WITH_GENDERS_IDS.includes(parseInt(pokemon.id))) {
        x += DEFAULT_WIDTH * (is_big_pokemon ? 4 : 2);
        const gendered_pokemon = format_pokemon(pokemon_data, x, y, is_big_pokemon);

        pokemons.push({ ...gendered_pokemon, gender: 2 });
    }

    x += DEFAULT_WIDTH * (is_big_pokemon ? 4 : 2);

    if (pokemon.name === "Unown") {
        x = 512;
        y = 1760;
        console.log(`Skipped ${pokemon.name} variants at ${x}, ${y}`);
    }

    if (pokemon.name === "Deoxys") {
        console.log(`Skipped ${pokemon.name} variants at ${x}, ${y}`);
        x = DEFAULT_WIDTH * 2;
        y += DEFAULT_HEIGHT * (big_pokemon_in_row ? 6 : 4);
    }

    if (x >= SPRITESHEET_WIDTH) {
        x = 0;
        y += DEFAULT_HEIGHT * (big_pokemon_in_row ? 6 : 4);
        big_pokemon_in_row = false;
    }
}

/**
 * Format the Pokemon data into a structured object.
 * @param {object} pokemon - The Pokemon data.
 * @param {number} x - The x-coordinate.
 * @param {number} y - The y-coordinate.
 * @param {boolean} is_big_pokemon - Indicates if the Pokemon is a big Pokemon.
 * @returns {object} The formatted Pokemon object.
 */
function format_pokemon(pokemon, x, y, is_big_pokemon) {
    return {
        id: parseInt(pokemon.id),
        name: pokemon.name,
        gender: pokemon.gender,
        types: [pokemon.type1, pokemon.type2].filter((type) => type !== ''),
        stats: {
            hp: parseInt(pokemon.hp),
            attack: parseInt(pokemon.attack),
            defense: parseInt(pokemon.defense),
            spAttack: parseInt(pokemon.spAttack),
            spDefense: parseInt(pokemon.spDefense),
            speed: parseInt(pokemon.speed),
        },
        properties: {
            walk_speed: 50,
            run_speed: 100,
            scale: 1,
            width: DEFAULT_WIDTH * (is_big_pokemon ? 2 : 1),
            height: DEFAULT_HEIGHT * (is_big_pokemon ? 2 : 1),
        },
        animations: [
            {
                preset: "walk_2x3_flipped",
                start: {
                    x,
                    y
                },
                size: {
                    width: DEFAULT_WIDTH * (is_big_pokemon ? 2 : 1),
                    height: DEFAULT_HEIGHT * (is_big_pokemon ? 2 : 1)
                }
            }
        ]
    };
}
