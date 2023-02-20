
template <class Type> // Function to call different selectivity parameterizations
// @param age = indexed age within age loop
// @param sel_model = integer of selectivity model 
// == 0, logistic
// @param ln_selpars vector of log selectivity parameters.
Type Get_Selex(int age, 
               int sel_model, 
               vector<Type> ln_selpars
                 ) {
  
  // Create container to return predicted selectivity value
  Type selex = 0;
  
  if(sel_model == 0) { // logistic selectivity
    // Extract out and exponentiate the parameters here
    Type a50 = exp(ln_selpars(0)); // a50
    Type k = exp(ln_selpars(1)); // slope
    selex = Type(1.0) / (Type(1) + exp(Type(-1) * (age - a50) / k));
  }
  
  if(sel_model == 1) { // gamma dome-shaped selectivity 
    // Extract out and exponentiate the parameters here
    Type delta = exp(ln_selpars(0)); // slope parameter
    Type amax = exp(ln_selpars(1)); // age at max selex
    
    // Now, calculate/derive power parameter + selex values
    Type p = 0.5 * (sqrt( pow(amax, 2) + (4 * pow(delta, 2))) - amax);
    selex = pow( (age / amax), (amax/p)  ) * exp( (amax - age) / p ); 
    
  }
  
  if(sel_model == 2) { // double logistic selectivity
    // Extract out and exponentiate the parameters here
    Type slope_1 = exp(ln_selpars(0)); // Slope of ascending limb
    Type slope_2 = exp(ln_selpars(1)); // Slope of descending limb
    Type infl_1 = exp(ln_selpars(2)); // Inflection point of ascending limb
    Type infl_2 = exp(ln_selpars(3)); // Inflection point of descending limb

    // Calculate logistic curve 1
    Type logist_1 = 1.0/(1.0 + exp(-slope_1 * (age - infl_1)));
    // Calculate logistic curve 2
    Type logist_2 = 1/(1.0 + exp(-slope_2 * (age - infl_2)));
    // Return selectivity - product of two logistic selectivity curves
    selex = logist_1 * (1 - logist_2);

  }
  
  if(sel_model == 3) { // exponential logistic (Thompson 1994)
    
    // Extract out and exponentiate the parameters here
    Type gamma = exp(ln_selpars(0)); 
    Type alpha = exp(ln_selpars(1)); 
    Type beta = exp(ln_selpars(2)); 
    
    // Define equations to minimize mistakes
    Type first = (1 / (1 - gamma));
    Type second = pow((1 - gamma) / gamma, gamma);
    Type third = exp( alpha * gamma * (beta - age ) );
    Type fourth = 1 + exp(alpha * (beta - age));

    // Return selectivity
    selex = first * second * (third/fourth);

  }
  
  
    
  return selex;
} // end function
