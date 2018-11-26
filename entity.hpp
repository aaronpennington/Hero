class Ent {
	public: 
		int x, y, ch; 
		TCODColor col;
		const char *name;
		bool blocks;

		std::shared_ptr<Combat> combat;
		std::shared_ptr<Mortal> mortal;
		std::shared_ptr<Ai> ai;

		Ent(int x, int y, int ch, const char *name, const TCODColor &col);
		void render() const;
		void update(std::shared_ptr<Ent> owner);
};