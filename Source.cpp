#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<vector>
#include<iostream>
#include<random>
#include<sstream>
#include<queue>

class GRID {

    static constexpr int matrixSize=25;
    static float gridsizeF; 
    static unsigned gridsizeU;
    float viewSPeed = 500.f;
    float x_off = (1920 - matrixSize * gridsizeF) / 2;
    float y_off = (1080 - matrixSize * gridsizeF) / 4;
    sf::View view;
    sf::Event event;
    sf::RenderWindow window;
    sf::RectangleShape obst_shape, path_shape;
    sf::Text obstacle, path_tracer,analyze;
    sf::Font font;
    std::vector<std::vector<sf::RectangleShape>>matrix;
    std::vector<int>coordinates;

public:
    //initialise a new window  and our grid elements(sqaures) and also our 2d grid   
    GRID(): window(sf::VideoMode({1920, 1080}), "GRID"), 
            path_shape(sf::Vector2f(gridsizeF, gridsizeF)),
            obst_shape(sf::Vector2f(gridsizeF, gridsizeF)){
                window.setFramerateLimit(1000);
                view.setSize({ 1920.f, 1080.f });
                view.setCenter(sf::Vector2(window.getSize().x / 2.f, window.getSize().y / 2.f));
    }
   
   void frame() {

       while (window.isOpen()) {       
   
           while (window.pollEvent(event)) {

               if (event.type == sf::Event::Closed) 
                   window.close();
           }
           
           //REnder/create a new frame/window
          window.clear(sf::Color::Color(41,162,198));  //backgroun
          window.setView(view);
       
           //print the matrix
           render_matrix();

           //resets view at the end of the drawing
         info_block();
         heading1();
          //  pathfinder();
           window.setView(window.getDefaultView());

           //render/display the newly created window
             window.display();
           
       }
    }

   void matrix_construction() {
       
       matrix.resize(matrixSize, std::vector<sf::RectangleShape>());

       for (int rpos = 0; rpos < matrixSize; rpos++) {

           matrix[rpos].resize(matrixSize, sf::RectangleShape());

           for (int cpos = 0; cpos < matrixSize; cpos++) {
              
               matrix[rpos][cpos].setSize({ gridsizeF,gridsizeF });
               matrix[rpos][cpos].setFillColor(sf::Color::White);
               matrix[rpos][cpos].setPosition({ x_off + rpos * gridsizeF, y_off + cpos * gridsizeF });

           }
       }

       srand((unsigned)time(NULL));
       int obstacleAmount = rand() % (4 * matrixSize);
       for (int i = 1; i < obstacleAmount; i++) {

           int r_xpos = rand() % matrixSize;
           int r_ypos = rand() % matrixSize;
           if (r_xpos ==0 && r_ypos == matrixSize-1)
               continue;
           matrix[r_xpos][r_ypos].setFillColor(sf::Color::Color(28, 29, 32));
           
       }
   }

   void render_matrix() {
       
    //   matrix_construction();
       
       for (int rpos = 0; rpos < matrixSize; rpos++) {
           for (int cpos = 0; cpos < matrixSize; cpos++) {
               window.draw(matrix[rpos][cpos]);
           }
       }
   }
  void heading1() {
      font.loadFromFile("fonts/font2.ttf");
      x_off = (1920 - matrixSize * gridsizeF) / 2;
      y_off = (1080 - matrixSize * (gridsizeF)*2);
       analyze.setCharacterSize(18);
       analyze.setFillColor(sf::Color::Color(237, 244, 245));
       analyze.setFont(font);
    //   analyze.setString("Analysing...");
    //   analyze.setPosition({ x_off+150, y_off+25});
       window.draw(analyze);
   }

  void info_block() {

       font.loadFromFile("fonts/font2.ttf");

       

        x_off = (1920 - matrixSize * gridsizeF) / 2;
        y_off = (1080 - matrixSize * (gridsizeF - 4));

       obstacle.setCharacterSize(18);
       obstacle.setFillColor(sf::Color::Color(237,244,245));
       obstacle.setFont(font);
       obstacle.setString("Obstacles");

       path_tracer.setCharacterSize(18);
       path_tracer.setFillColor(sf::Color::Color(237, 244, 245));
       path_tracer.setFont(font);
       path_tracer.setString("Path tracer");
       
       obst_shape.setPosition({ x_off, y_off});
       obst_shape.setFillColor(sf::Color::Color(28, 29, 32));

       path_shape.setPosition({ x_off, y_off+50});
       path_shape.setFillColor(sf::Color::Color(239, 113, 38));

       obstacle.setPosition({ x_off+50, y_off });
       path_tracer.setPosition({x_off+50, y_off+50});

       window.draw(obst_shape);
       window.draw(path_shape);

       window.draw(obstacle);
       window.draw(path_tracer);
 
   }

  void pathfinder() {
      x_off = (1920 - matrixSize * gridsizeF) / 2;
      y_off = (1080 - matrixSize * (gridsizeF) * 2);
      //to update the indexes of the current block  
      coordinates.resize(2, 0);

      //queue to keep track of the path
      std::queue<std::vector<int>>currblock;
      
      //vector to store the block which can be visited or not
      bool can_visit[matrixSize][matrixSize];
      bool has_visited[matrixSize][matrixSize];
      std::vector<std::vector<std::vector<int>>> pred(matrixSize, std::vector<std::vector<int>>(matrixSize));

      //traversing through whole grid to check which blocks can be visited
      for (int i = 0; i < matrixSize; i++) {
          for (int j = 0; j < matrixSize; j++) {
              if (matrix[i][j].getFillColor() == sf::Color::Color(28, 29, 32))
                  can_visit[i][j] = false;
              else
                  can_visit[i][j] = true;
          }
      }
      for (int i = 0; i < matrixSize; i++) {
          for (int j = 0; j < matrixSize; j++) {
              has_visited[i][j] = false;
          }
      }

      //BFS starts
      //marking the starting point of the grid 
      coordinates[1] = matrixSize - 1;
      can_visit[coordinates[0]][coordinates[1]] = true;
      has_visited[coordinates[0]][coordinates[1]] = true;
      
      currblock.push(coordinates);
      pred[0][matrixSize-1] = { -1,-1 };

      //start point of the grid
      matrix[0][matrixSize - 1].setFillColor(sf::Color::Blue);

      //end point of the grid
      matrix[matrixSize - 1][0].setFillColor(sf::Color::Color(239,113,38)); //green

      while (!currblock.empty()) {

          auto curr = currblock.front();
          currblock.pop();
          
          int x_pos = curr[0];
          int y_pos = curr[1];

          analyze.setPosition({ x_off + 150, y_off + 25 });
          analyze.setString("Analysing...");

          if (x_pos == matrixSize-1 && y_pos == 0) {
              while (true) {
                  matrix[x_pos][y_pos].setFillColor(sf::Color::Color(239, 113, 38));
                  x_off = (1920 - matrixSize * gridsizeF) / 2;
                  y_off = (1080 - matrixSize * (gridsizeF) * 2);
                  analyze.setPosition({ x_off+60 , y_off + 25 });
                  analyze.setString("Analyzation complete");
                  final_render();
                  
                  auto p = pred[x_pos][y_pos];
                  if (p == std::vector({ -1, -1 }))
                      break;
                  x_pos = p[0]; y_pos = p[1];
              }
              break;
          }

          //moving up
          if ((y_pos - 1 >= 0) && can_visit[x_pos][y_pos - 1] == true && has_visited[x_pos][y_pos - 1] == false) {
              coordinates[0] = x_pos;
              coordinates[1] = y_pos - 1;
              has_visited[coordinates[0]][coordinates[1]] = true;
              currblock.push(coordinates);
              matrix[x_pos][y_pos - 1].setFillColor(sf::Color::Color(192,192,192));
              pred[coordinates[0]][coordinates[1]] = { x_pos, y_pos };
              final_render();
          }

          //moving down
          if ((y_pos + 1 < matrixSize) && can_visit[x_pos][y_pos + 1] == true && has_visited[x_pos][y_pos + 1] == false) {
              coordinates[0] = x_pos;
              coordinates[1] = y_pos + 1;
              has_visited[coordinates[0]][coordinates[1]] = true;
              currblock.push(coordinates);
              matrix[x_pos][y_pos + 1].setFillColor(sf::Color::Color(192, 192, 192));
              pred[coordinates[0]][coordinates[1]] = { x_pos, y_pos };
              final_render();
          }

          //moving right
          if ((x_pos + 1 < matrixSize) && can_visit[x_pos + 1][y_pos] == true && has_visited[x_pos + 1][y_pos] == false) {
              coordinates[0] = x_pos + 1;
              coordinates[1] = y_pos;
              has_visited[coordinates[0]][coordinates[1]] = true;
              currblock.push(coordinates);
              matrix[x_pos + 1][y_pos].setFillColor(sf::Color::Color(192, 192, 192));
              pred[coordinates[0]][coordinates[1]] = { x_pos, y_pos };
              final_render();
          }

          //moving left
          if ((x_pos - 1 >= 0) && can_visit[x_pos - 1][y_pos] == true && has_visited[x_pos - 1][y_pos] == false) {
              coordinates[0] = x_pos - 1;
              coordinates[1] = y_pos;
              has_visited[coordinates[0]][coordinates[1]] = true;
              currblock.push(coordinates);
              matrix[x_pos - 1][y_pos].setFillColor(sf::Color::Color(192, 192, 192));
              pred[coordinates[0]][coordinates[1]] = { x_pos, y_pos };
              final_render();
          
          }

      }

  }
  void final_render() {
      
      window.clear(sf::Color::Color(41,162,198));
      render_matrix();
      info_block();
      heading1();
      window.display();

  }

}gridobj;

float GRID::gridsizeF = 20.f;
unsigned GRID::gridsizeU = static_cast<unsigned>(gridsizeF);

int main()
{   
    gridobj.matrix_construction();
    gridobj.pathfinder();
    gridobj.frame();
    return 0;

}