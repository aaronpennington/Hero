class Engine {
    public:
        std::vector<std::shared_ptr<Ent>> entL;
		std::shared_ptr<Ent> player;
		std::unique_ptr<Map> dungeon;
		Gui *gui;

		int fovRad, sW, sH;
		TCOD_key_t lastKey;
		TCOD_mouse_t mouse;

        Engine(int sW, int sH); 
		~Engine();

        void update(); 
		void render();

		enum gameState { START, IDLE, TURN, WIN, LOSE } gameState;

	private:
		bool computeFov;
};

    extern Engine engine;