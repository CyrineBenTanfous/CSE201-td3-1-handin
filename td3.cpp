#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double *new_array;
  new_array = new double[new_size];
  for (int i = 0; i < new_size; ++i){
      if (i < length)
          new_array[i] = array[i];
      else
          new_array[i] = 0;
  }
  delete[] array;
  return new_array;
  // IMPLEMENT YOUR FUNCTION HERE

  return NULL; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
    double *new_array;
    new_array = new double[new_size];
    for (int i = 0; i < new_size; i++){
        if (i<length)
            new_array[i] = array[i];
        else
            new_array = extend_array(array, new_size, length);
        }
    delete[] array;
    return new_array;
    // IMPLEMENT YOUR FUNCTION HERE

  return NULL; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
    if (current_size == max_size){
        array = extend_array(array, current_size, max_size+5);
        max_size += 5;
    }
    array[current_size] = element;
    current_size += 1;
    return array;

    // IMPLEMENT YOUR FUNCTION HERE
  return NULL; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
    if (current_size>0){
        current_size -=1;
      if ((max_size - current_size) >= 5){
          int new_size = max_size - 5;
          array = shrink_array(array, current_size, new_size);
          max_size = new_size;
                  }
    }


  return array;// IMPLEMENT YOUR FUNCTION HERE

  return NULL; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
  telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
  telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;

      if (y >= 0) {
          telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
          telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
          telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
            }
    }
  }

  return hit_target;
}



void sort_telemetries(double *global_telemetry, int &global_telemetry_current_size) {
  for(int i = 0; i < global_telemetry_current_size; i = i+3)
      for(int j = 0; j < global_telemetry_current_size-3; j = j+3){
          if (global_telemetry[j] > global_telemetry[j+3]){
              double n = global_telemetry[j];
              double m = global_telemetry[j+1];
              double l = global_telemetry[j+2];
              global_telemetry[j] = global_telemetry[j+3];
              global_telemetry[j+1] = global_telemetry[j+4];
              global_telemetry[j+2] = global_telemetry[j+5];
              global_telemetry[j+3] = n;
              global_telemetry[j+4] = m;
              global_telemetry[j+5] = l;
          }
      }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size){

  for (int i = 0; i < tot_telemetries; i++){
    for (int j = 0; j < telemetries_sizes[i]; j++){
        global_telemetry = append_to_array(telemetries[i][j],
                                  global_telemetry,
                                  global_telemetry_current_size,
                                  global_telemetry_max_size);
    }
  }
//  std::cout<< "global_telemetry[0]"<<global_telemetry[0]<<std::endl;
//  std::cout<< "global_telemetry[1]"<<global_telemetry[1]<<std::endl;
//  std::cout<< "global_telemetry[2]"<<global_telemetry[2]<<std::endl;
//  std::cout<< "global_telemetry[3]"<<global_telemetry[3]<<std::endl;

  sort_telemetries(global_telemetry, global_telemetry_current_size);
}

