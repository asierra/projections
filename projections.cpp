/* This file is part of Projections, an OpenGL interactive and
   educative application about world projections.
   Copyright (c) 2018  Alejandro Aguilar Sierra (asierra@unam.mx)
*/
#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <set>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int image_width, image_height, image_channels;
unsigned char *image_data;

bool render_wire = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 800;

const int grid_width  = 180;
const int grid_height = 90;
std::size_t const vertex_total = grid_width * grid_height;
std::size_t const index_total = (grid_width - 1) * (grid_height - 1) * 6;

glm::vec3 vertices[vertex_total];
unsigned int indices[index_total];

#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <vector>
std::vector<std::string> projections;
unsigned int projection_idx=0;
std::set<std::string> geometrics;

void read_directory(const std::string& dirname)
{
  DIR* dirp = opendir(dirname.c_str());
  struct dirent * dp;
  while ((dp = readdir(dirp)) != NULL) {
    std::string str = dp->d_name;
    std::size_t foundvs = str.find(".vs");
    std::size_t foundgs = str.find(".gs");
    if (foundvs!=std::string::npos) {
      projections.push_back("shaders/"+str);
    } else if (foundgs!=std::string::npos) {
      geometrics.insert("shaders/"+str);
    }
  }
  closedir(dirp);
}

std::string make_title()
{
  std::string str = projections[projection_idx];
  std::size_t s = str.find("/");
  std::size_t e = str.find(".");
  str = str.substr(s+1, e-s-1);
  str[0] -= 32;
  return "World Projection: "+str;
}
  
void createGrid() {
  int vertex_count = 0; 
  int index_count = 0;
  float x, y;
  
  std::cout<<"Contruyendo rejilla."<<std::endl;
  for (int j=0;  j < grid_height; j++) {
    y = -1.0 + 2.0*j/(grid_height - 1.0);
    for (int i=0; i < grid_width; i++) {
      x = -1 + 2.0*i/(grid_width - 1.0);
      vertices[vertex_count] = glm::vec3(x, y, 0.0);      
        if (i < grid_width - 1) {
	if (j > 0.0) {
	  indices[index_count++] = vertex_count - grid_width;
	  indices[index_count++] = vertex_count + 1;
	  indices[index_count++] = vertex_count;
	}
	if (j < grid_height - 1.0) {
	  indices[index_count++] = vertex_count;
	  indices[index_count++] = vertex_count + 1;
	  indices[index_count++] = vertex_count + grid_width + 1;
	}
      }	
      vertex_count++;
    }
  }
  std::cout<<"Total vertices "<< vertex_count << "  " << vertex_total << std::endl;
  std::cout<<"Total indices  "<< index_count << "  " << index_total << std::endl;
}


int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "World Projections", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
    
  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;    

  read_directory("shaders/");
  std::cout << "Number of projections " << projections.size() << std::endl;
  std::vector<Shader> shaders;
  for( int i=0; i<projections.size(); ++i) {
    std::cout << projections[i];
    std::string vs = projections[i];
    std::string gs = vs.substr(0, vs.size()-3) + ".gs";
    std::cout << " Shader " << gs << std::endl;
    if (geometrics.find(gs) != geometrics.end())
      shaders.push_back(Shader(vs.c_str(), "shaders/texture.fs", gs.c_str()));
    else
      shaders.push_back(Shader(vs.c_str(), "shaders/texture.fs"));
  }

  createGrid();
    
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);  
  glBindVertexArray(VAO);  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
    
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  image_data = stbi_load("earth_2k.jpg", &image_width, &image_height,
			 &image_channels, 0);
  if (image_data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(image_data);
    
  // render loop
  while (!glfwWindowShouldClose(window)) {
    if (render_wire)
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    shaders[projection_idx].use();

    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, index_total, GL_UNSIGNED_INT, 0);
 
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  
  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
    
  if (key == GLFW_KEY_W && action == GLFW_PRESS)
    render_wire = !render_wire;
  
  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    projection_idx++;
    if (projection_idx==projections.size())
      projection_idx = 0;
    glfwSetWindowTitle(window, make_title().c_str());
  }
  
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    if (projection_idx == 0)
      projection_idx = projections.size()-1;
    else
      projection_idx--;
    glfwSetWindowTitle(window, make_title().c_str());
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
