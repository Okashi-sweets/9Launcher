.import "fs.js" as File

function populateGamesList(jsonFile) {
    const games = File.loadJSON("../json/" + jsonFile);
    if (games == null) {
        throw new Error(jsonFile + " failed to load!");
    }

    window.mainModel = [].concat(
        Object.values(games.games),
    );

    if (jsonFile == "games.json") {
        window.isOfficialGames = true;
        window.pc98Model = window.mainModel.filter(game => game.isPC98 === true);
        window.windowsModel = window.mainModel.filter(game => game.isPC98 !== true);
        window.spinoffModel = Object.values(games.spinoffs);
    } else {
        window.isOfficialGames = false;
        window.pc98Model = [];
        window.windowsModel = window.mainModel;
        window.spinoffModel = [];
    }
}
