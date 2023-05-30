const fs = require('fs');

if (process.argv.length < 3) {
    console.log('Usage: node create_pokemons.js <file_path>');
    process.exit(1);
}

const filePath = process.argv[2];

const pokemons = [];

const width = process.argv[3] || 32;
const height = process.argv[4] || 32;



let x = 0;
let y = 0;

let spritesheet_width = 1024;
let spritesheet_height = 5408;

const pokemonWithGendersIds = [3, 25, 154, 202, 208, 214, 415, 443, 444, 445, 449, 450];

fs.readFileSync(filePath, 'utf8').split('\n').forEach((line) => {
    // 001	Bulbasaur	Grass	Poison	45	49	49	65	65	45
    const [id, name, type1, type2, hp, attack, defense, spAttack, spDefense, speed] = line.split('\t');

    const pokemonData = { id, name, type1, type2, hp, attack, defense, spAttack, spDefense, speed, gender: 1 };

    let pokemon = formatPokemon(pokemonData, x, y);
    pokemons.push(pokemon);

    if (pokemonWithGendersIds.includes(pokemon.id)) {
        x += width * 2;
        pokemon = formatPokemon(pokemonData, x, y);
        pokemons.push({ ...pokemon, gender: 2 });
    }

    x += width * 2;
    if (x >= spritesheet_width) {
        x = 0;
        y += height;
    }
});

//  JSON.stringify(pokemons, null, 2);

const json = {
    texture_name: "bw_old_overworld.png",
    pokemons
}

fs.writeFileSync('src/assets/animations/pokemons/bw_old_overworld.json', JSON.stringify(json, null, 2));

function formatPokemon(pokemon, x, y) {
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
            width,
            height
        },
        animations: [
            {
                preset: "walk_2x3_flipped",
                start: {
                    x,
                    y
                },
                size: {
                    width,
                    height
                }
            }
        ]
    }
}