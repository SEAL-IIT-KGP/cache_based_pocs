import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

# Read data
cache_hits = open("hits").readlines()
cache_misses = open("misses").readlines()

# Preprocess
index = 0
for cache_hit in cache_hits:
    cache_hit = cache_hit.strip()
    cache_hit = cache_hit.split(" ")
    cache_hits[index] = int(cache_hit[2])
    index = index + 1

index = 0
for cache_miss in cache_misses:
    cache_miss = cache_miss.strip()
    cache_miss = cache_miss.split(" ")
    cache_misses[index] = int(cache_miss[2])
    index = index + 1

sns.histplot(cache_hits, label="Cache Hits", kde=True, bins='auto', alpha=0.2, kde_kws={'bw_adjust':1.5}, color='red')
sns.histplot(cache_misses, label="Cache Misses", kde=True, bins='auto', alpha=0.2, kde_kws={'bw_adjust':1.5}, color='blue')
plt.xlabel("rdtsc measurements")
plt.ylabel("Density")
plt.legend()
plt.show()
