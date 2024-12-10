#include "Map.h"

Map::Map(std::string name, Map* parent) : parent_(parent), name_(name)
{
}

void Map::addChild(Map* child)
{
	// Jos löytyy jo
	for (Map* saved : children_) {
		if (saved->getName() == child->getName()) return;
	}

	children_.push_back(child);
}

void Map::setWLD(std::vector<int> v)
{
	W_ = v.at(0);
	L_ = v.at(1);
	D_ = v.at(2);
}

void Map::addWin(std::ostream& output)
{
	addGame(output, RESULTS(W));
}

void Map::addLoss(std::ostream& output)
{
	addGame(output, RESULTS(L));
}

void Map::addDraw(std::ostream& output)
{
	addGame(output, RESULTS(D));
}

void Map::delWin(std::ostream& output)
{
	delGame(output, RESULTS(W));
}

void Map::delLoss(std::ostream& output)
{
	delGame(output, RESULTS(L));
}

void Map::delDraw(std::ostream& output)
{
	delGame(output, RESULTS(D));
}

std::vector<int> Map::getWLD() const
{
	return { W_, L_, D_ };
}

int Map::getWins() const
{
	return W_;
}

int Map::getLosses() const
{
	return L_;
}

int Map::getDraws() const
{
	return D_;
}

int Map::getGames() const
{
	return W_ + L_ + D_;
}

std::string Map::getName() const
{
	return name_;
}

Map* Map::getMapt() const
{
	return parent_;
}

std::vector<Map*> Map::getChildren() const
{
	return children_;
}

double Map::getWinrate() const
{
	int total = getGames() - getDraws();

	if (total == 0) return 0;

	double wins = 1.0 * getWins();

	return wins / total * 100;
}

std::string Map::getOutput(const std::string::size_type& maplen) const
{
	std::string to_return = "";

	to_return.append(getName());
	to_return.append(": ");

	while (to_return.length() <= maplen) {
		to_return.append(" ");
	}

	to_return.append(std::to_string(getWins()));
	to_return.append("/");
	to_return.append(std::to_string(getLosses()));
	to_return.append("/");
	to_return.append(std::to_string(getDraws()));

	while (to_return.length() <= maplen + 13) {
		to_return.append(" ");
	}

	to_return.append(" W-%: ");
	to_return.append(std::to_string(getWinrate()));

	return to_return;
}

void Map::addGame(std::ostream& output, const int& result)
{
	if (result == RESULTS(W)) {
		++W_;
		output << "Succesfully changed wins for " << getName() << " to "
			<< getWins() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->addWin(output);
		}
		return;
	}

	if (result == RESULTS(L)) {
		++L_;
		output << "Succesfully changed losses for " << getName() << " to "
			<< getLosses() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->addLoss(output);
		}
		return;
	}

	if (result == RESULTS(D)) {
		++D_;
		output << "Succesfully changed draws for " << getName() << " to "
			<< getDraws() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->addDraw(output);
		}
	}
}

void Map::delGame(std::ostream& output, const int& result)
{
	/*
	* TODO:
	* 
	* Suostuu aloittamaan poistamisen, vaikka 'vanhemmilla' ei olisi tarpeeksi
	* kyseisiä pelejä. Tämän tilan ei pitäisi olla mahdollinen, mutta joku tarkastus
	* olisi varmasti hyvä
	* 
	*/

	if (result == RESULTS(W)) {
		if (W_ <= 0) {
			output << "Error! " << getName() << " has too few wins!"
				<< std::endl;
			return;
		}

		--W_;
		output << "Succesfully changed wins for " << getName() << " to "
			<< getWins() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->delWin(output);
		}
		return;
	}

	if (result == RESULTS(L)) {
		if (L_ <= 0) {
			output << "Error! " << getName() << " has too few losses!"
				<< std::endl;
			return;
		}

		--L_;
		output << "Succesfully changed losses for " << getName() << " to "
			<< getLosses() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->delLoss(output);
		}
		return;
	}

	if (result == RESULTS(D)) {
		if (D_ <= 0) {
			output << "Error! " << getName() << " has too few draws!"
				<< std::endl;
			return;
		}

		--D_;
		output << "Succesfully changed draws for " << getName() << " to "
			<< getDraws() << "!" << std::endl;
		if (parent_ != nullptr) {
			parent_->delDraw(output);
		}
	}
}
