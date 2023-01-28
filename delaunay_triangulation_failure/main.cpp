#include "include/structures.h"

int framecount = 0;
const int WIDTH = 600;
const int HEIGHT = 600;

const int pts_nbr = 20;
std::array<sf::Vector2f, pts_nbr> points;

std::vector<_triangle> point_triangulation;

std::vector<segment> getUniqueEdges(std::vector<segment> edges) {
    std::vector<segment> uniqueEdges;

    for (int i = 0; i < edges.size(); i++){
        bool is_uniqe = true;

        for (int j = 0; j < edges.size(); ++j){
            if (i != j && edges[i].equals(edges[j])){
                is_uniqe = false;
                break;
            }            
        }

        if(is_uniqe) {
            uniqueEdges.push_back(edges[i]);
        }
    }

    return uniqueEdges;
}

std::vector<_triangle> addVertex(sf::Vector2f vertex,std::vector<_triangle> triangulation) {
    std::vector<segment> edges; //bad triangles in the algorithm
    // triangulation.reserve(4*pts_nbr);

    // add to bad triangles and delete from triangulation
    for(int i = 0; i < triangulation.size(); ++i){
        if(triangulation[i].inCircumcircle(vertex)) {
            //add to bad tirangles
            edges.push_back(segment(triangulation[i].v0, triangulation[i].v1));
            edges.push_back(segment(triangulation[i].v1, triangulation[i].v2));
            edges.push_back(segment(triangulation[i].v2, triangulation[i].v0));

            triangulation.erase(triangulation.begin()+i-1);

        }
    }

    std::cout << triangulation.capacity() << " \n";
    //get the polygone edges(unique 1s)
    edges = getUniqueEdges(edges);    

    //create new triangles from the unique edges
    for (int i = 0; i < edges.size(); ++i){
        _triangle new_triangle (edges[i].v0, edges[i].v1, vertex);
        triangulation.push_back(new_triangle);
        std::cout << "new_triangle pushed" << " \n";
    }


    return triangulation;
}

std::vector<_triangle> campute_triangulation(std::array<sf::Vector2f, pts_nbr> vertices) {
    std::vector<_triangle> triangulation;
    //create the super _triangle
    _triangle st = calcSuperTriangle(vertices);
    triangulation.push_back(st);


    //add vertices to the tiangulation
    for(sf::Vector2f &vertex : vertices){
        triangulation = addVertex(vertex, triangulation);
std::cout << "vertex was added"<< " \n";

    }

    //clean up(remove any tiangle sharing a vertex with the super _triangle)
    // triangulation = filtre_from(st, triangulation);

    return triangulation;
}

int main()
{
    srand (time(NULL));

    for (int i = 0; i < pts_nbr; ++i){
        points[i] = sf::Vector2f(rand() % WIDTH, rand() % HEIGHT);
    }
    point_triangulation.reserve(4*pts_nbr);
   point_triangulation = campute_triangulation(points);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update part

        window.clear();
        //drawing part
            //points drawing
        for(int i = 0; i < pts_nbr; i++) {
            sf::CircleShape circle;
            circle.setRadius(3);
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(0);
            circle.setPosition(points[i].x, points[i].y);
            window.draw(circle);
        }

            //drawing the triangles
        for (int i = 0; i < point_triangulation.size(); ++i){
            sf::ConvexShape convex;

            convex.setPointCount(3);
            convex.setPoint(0, point_triangulation[i].v0);
            convex.setPoint(1, point_triangulation[i].v1);
            convex.setPoint(2, point_triangulation[i].v2);

            window.draw(convex);
        }

        window.display();

        // std::cout << std::to_string(framecount) + " \n";
        framecount++;
    }

    return 0;
}
