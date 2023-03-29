import pandas as pd
import numpy as np
import matplotlib.pyplot as plt 
import seaborn as sns 
%matplotlib inline
from sklearn.base import BaseEstimator,TransformerMixin

class OutlierHandler(BaseEstimator,TransformerMixin):
    """
    Description:
    Detects and Handles Outliers.
    
    Parameters:
    strategy = {'trim', 'cap', 'nan'} default = 'cap'.
    strategy sets how outliers will be dealt with after detection. 
        For strategy='trimming':
            Instances with outliers will be dropped (Not recommended).
        For strategy='capping':
            Instances with outliers will be "capped" or replaced with upper and lower limit
            values computed according to the method chosen.
        For strategy='nan':
            Instnaces with outliers will have the positions of outliers replaced with np.nan
            "NaN" values. This data can then be treated and imputed using any of the various 
            imputation techniques.
    
    method = {'z_score', 'iqr', 'percentile'}
    method sets how outliers will be detected. default = 'iqr'.
    Depending on method chosen, you will need to pass additional parameter(s).
        For method='z_score': 
            Pass Standard Deviation 'std' above and below which outliers will be 
            detected and handles according to strategy chosen. Note that this method is 
            optimally used only for normally distributed features.
            default=3.
        For method='iqr':
            Pass 'factor' by which IQR needs to be multiplied by for computing upper and lower limits.
            default=1.5.
        For method='percentile':
            Pass 'alpha' which will be the percentile that will be used to detect outliers and handle it 
            according to the method chosen.
            default=0.01.
    
    Returns:
    numpy.ndarray of transformed data with outliers detected and handles 
    according to method and strategy chosen respectively.
    """
    
    def _init_(self, strategy = 'cap', method = 'iqr', factor=1.5, zstd=3, alpha=0.01):
        self.factor = factor
        self.strategy = strategy
        self.method = method
        self.factor = factor
        self.factor = factor
        self.zstd = zstd
        self.alpha = alpha
        
    def outlier_iqr(self,X,y=None):
        X = pd.Series(X).copy()
        self.q1 = X.quantile(0.25)
        self.q3 = X.quantile(0.75)
        self.iqr = self.q3 - self.q1
        self.lower_bound = self.q1 - (self.factor * self.iqr)
        self.upper_bound = self.q3 + (self.factor * self.iqr)
        if self.strategy == 'nan':
            X.loc[((X < self.lower_bound) | (X > self.upper_bound))] = np.nan 
        elif self.strategy == 'trim':
            X = X.loc[((X > self.lower_bound) & (X < self.upper_bound))]
        else:
            X = np.where((X > self.upper_bound), self.upper_bound, np.where((X < self.lower_bound), self.lower_bound, X))
        return pd.Series(X)
    
    def outlier_zscore(self,X,y=None):
        X = pd.Series(X).copy()
        self.mean = X.mean()
        self.std = X.std()
        self.lower_bound = self.mean - (self.zstd * self.std)
        self.upper_bound = self.mean + (self.zstd * self.std)
        if self.strategy == 'nan':
            X.loc[((X < self.lower_bound) | (X > self.upper_bound))] = np.nan 
        elif self.strategy == 'trim':
            X = X.loc[((X > self.lower_bound) & (X < self.upper_bound))]
        else:
            X = np.where((X > self.upper_bound), self.upper_bound, np.where((X < self.lower_bound), self.lower_bound, X))
        return pd.Series(X)
    
    def outlier_percentile(self,X,y=None):
        X = pd.Series(X).copy()
        self.lower_bound = X.quantile(0.00+self.alpha)
        self.upper_bound = X.quantile(1.00-self.alpha)
        if self.strategy == 'nan':
            X.loc[((X < self.lower_bound) | (X > self.upper_bound))] = np.nan 
        elif self.strategy == 'trim':
            X = X.loc[((X > self.lower_bound) & (X < self.upper_bound))]
        else:
            X = np.where((X > self.upper_bound), self.upper_bound, np.where((X < self.lower_bound), self.lower_bound, X))
        return pd.Series(X)
    
    def fit(self,X,y=None):
        return self
    
    def transform(self,X,y=None):
        if self.method == 'iqr':
            return X.apply(self.outlier_iqr)
        elif self.method == 'z_score':
            return X.apply(self.outlier_zscore)
        else:
            return X.apply(self.outlier_percentile)
