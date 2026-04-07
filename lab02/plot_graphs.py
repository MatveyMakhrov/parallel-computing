import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Создаем директории для графиков
os.makedirs('plots', exist_ok=True)

plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['font.size'] = 12
plt.rcParams['axes.labelsize'] = 14
plt.rcParams['axes.titlesize'] = 16
plt.rcParams['legend.fontsize'] = 12

# Определяем базовый путь к результатам
RESULTS_PATH = 'build/results/'

print("Generating algorithm comparison plot...")
try:
    df = pd.read_csv(os.path.join(RESULTS_PATH, 'algorithms_comparison.csv'))
    
    plt.figure(figsize=(12, 8))
    plt.plot(df['N'], df['Classic'], 'o-', label='Classic', linewidth=2, markersize=8)
    plt.plot(df['N'], df['Transposed'], 's-', label='Transposed', linewidth=2, markersize=8)
    plt.plot(df['N'], df['Buffered'], '^-', label='Buffered', linewidth=2, markersize=8)
    plt.plot(df['N'], df['Blocked'], 'd-', label='Blocked (S=64)', linewidth=2, markersize=8)
    
    plt.xlabel('Matrix Size N')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Matrix Multiplication Performance Comparison')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.xticks(df['N'], df['N'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/algorithms_comparison.png', dpi=150)
    plt.savefig('plots/algorithms_comparison.pdf')
    print("  Saved: plots/algorithms_comparison.png")
except FileNotFoundError:
    print(f"  ERROR: File not found: {os.path.join(RESULTS_PATH, 'algorithms_comparison.csv')}")
except Exception as e:
    print(f"  ERROR: {e}")

print("\nGenerating block size dependency plot...")
try:
    df_block = pd.read_csv(os.path.join(RESULTS_PATH, 'block_size_dependency.csv'))
    
    plt.figure(figsize=(10, 6))
    plt.semilogx(df_block['BlockSize'], df_block['GFLOPs'], 'o-', linewidth=2, markersize=8, color='green')
    plt.xlabel('Block Size')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Performance vs Block Size (N = 1024)')
    plt.grid(True, alpha=0.3)
    plt.xticks(df_block['BlockSize'], df_block['BlockSize'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/block_size_dependency.png', dpi=150)
    plt.savefig('plots/block_size_dependency.pdf')
    print("  Saved: plots/block_size_dependency.png")
except FileNotFoundError:
    print(f"  ERROR: File not found: {os.path.join(RESULTS_PATH, 'block_size_dependency.csv')}")
except Exception as e:
    print(f"  ERROR: {e}")

print("\nGenerating unroll buffered plot...")
try:
    df_unroll_buff = pd.read_csv(os.path.join(RESULTS_PATH, 'unroll_buffered.csv'))
    
    plt.figure(figsize=(10, 6))
    plt.plot(df_unroll_buff['UnrollFactor'], df_unroll_buff['GFLOPs'], 's-', linewidth=2, markersize=8, color='blue')
    plt.xlabel('Unroll Factor M')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Performance vs Unroll Factor (Buffered Method, N = 1024)')
    plt.grid(True, alpha=0.3)
    plt.xticks(df_unroll_buff['UnrollFactor'], df_unroll_buff['UnrollFactor'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/unroll_buffered.png', dpi=150)
    plt.savefig('plots/unroll_buffered.pdf')
    print("  Saved: plots/unroll_buffered.png")
except FileNotFoundError:
    print(f"  ERROR: File not found: {os.path.join(RESULTS_PATH, 'unroll_buffered.csv')}")
except Exception as e:
    print(f"  ERROR: {e}")

print("\nGenerating unroll blocked plot...")
try:
    df_unroll_block = pd.read_csv(os.path.join(RESULTS_PATH, 'unroll_blocked.csv'))
    
    plt.figure(figsize=(10, 6))
    plt.plot(df_unroll_block['UnrollFactor'], df_unroll_block['GFLOPs'], 'd-', linewidth=2, markersize=8, color='orange')
    plt.xlabel('Unroll Factor M')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Performance vs Unroll Factor (Blocked Method, N = 1024)')
    plt.grid(True, alpha=0.3)
    plt.xticks(df_unroll_block['UnrollFactor'], df_unroll_block['UnrollFactor'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/unroll_blocked.png', dpi=150)
    plt.savefig('plots/unroll_blocked.pdf')
    print("  Saved: plots/unroll_blocked.png")
except FileNotFoundError:
    print(f"  ERROR: File not found: {os.path.join(RESULTS_PATH, 'unroll_blocked.csv')}")
except Exception as e:
    print(f"  ERROR: {e}")

print("\nGenerating optimized scaling plot...")
try:
    df_opt = pd.read_csv(os.path.join(RESULTS_PATH, 'optimized_scaling.csv'))
    
    plt.figure(figsize=(12, 8))
    plt.plot(df_opt['N'], df_opt['Buffered_Opt'], 's-', label='Buffered (M=optimal)', linewidth=2, markersize=8)
    plt.plot(df_opt['N'], df_opt['Blocked_Opt'], 'd-', label='Blocked (S=optimal, M=optimal)', linewidth=2, markersize=8)
    
    plt.xlabel('Matrix Size N')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Performance Scaling for Optimized Implementations')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.xticks(df_opt['N'], df_opt['N'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/optimized_scaling.png', dpi=150)
    plt.savefig('plots/optimized_scaling.pdf')
    print("  Saved: plots/optimized_scaling.png")
except FileNotFoundError:
    print(f"  ERROR: File not found: {os.path.join(RESULTS_PATH, 'optimized_scaling.csv')}")
except Exception as e:
    print(f"  ERROR: {e}")

print("\nGenerating combined plot...")
try:
    df = pd.read_csv(os.path.join(RESULTS_PATH, 'algorithms_comparison.csv'))
    df_opt = pd.read_csv(os.path.join(RESULTS_PATH, 'optimized_scaling.csv'))
    
    plt.figure(figsize=(14, 8))
    
    plt.plot(df['N'], df['Classic'], 'o-', label='Classic', linewidth=1.5, markersize=6, alpha=0.7)
    plt.plot(df['N'], df['Transposed'], 's-', label='Transposed', linewidth=1.5, markersize=6, alpha=0.7)
    plt.plot(df['N'], df['Buffered'], '^-', label='Buffered', linewidth=1.5, markersize=6, alpha=0.7)
    plt.plot(df['N'], df['Blocked'], 'd-', label='Blocked (S=64)', linewidth=1.5, markersize=6, alpha=0.7)
    plt.plot(df_opt['N'], df_opt['Buffered_Opt'], 's--', label='Buffered (M=optimal)', linewidth=2.5, markersize=8, color='blue')
    plt.plot(df_opt['N'], df_opt['Blocked_Opt'], 'd--', label='Blocked (S=optimal, M=optimal)', linewidth=2.5, markersize=8, color='red')
    
    plt.xlabel('Matrix Size N')
    plt.ylabel('Performance (GFLOP/s)')
    plt.title('Matrix Multiplication: All Implementations')
    plt.legend(loc='best')
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.xticks(df['N'], df['N'].astype(str))
    plt.tight_layout()
    plt.savefig('plots/all_implementations.png', dpi=150)
    plt.savefig('plots/all_implementations.pdf')
    print("  Saved: plots/all_implementations.png")
except FileNotFoundError:
    print(f"  ERROR: One or more files not found in {RESULTS_PATH}")
except Exception as e:
    print(f"  ERROR: {e}")

# Проверяем, какие файлы действительно существуют
print("\n" + "="*50)
print("Checking for existing CSV files...")
print("="*50)
if os.path.exists(RESULTS_PATH):
    csv_files = [f for f in os.listdir(RESULTS_PATH) if f.endswith('.csv')]
    if csv_files:
        print(f"Found CSV files in {RESULTS_PATH}:")
        for f in csv_files:
            print(f"  - {f}")
    else:
        print(f"No CSV files found in {RESULTS_PATH}")
else:
    print(f"Directory {RESULTS_PATH} does not exist!")
    
print("\n" + "="*50)
print("All graphs saved to 'plots/' directory!")
print("Files generated:")
print("  - algorithms_comparison.png")
print("  - block_size_dependency.png")
print("  - unroll_buffered.png")
print("  - unroll_blocked.png")
print("  - optimized_scaling.png")
print("  - all_implementations.png")
print("="*50)

print("\nGenerating step plots...")

for file in os.listdir(RESULTS_PATH):
    if file.startswith("step_N_") and file.endswith(".csv"):
        df_step = pd.read_csv(os.path.join(RESULTS_PATH, file))

        plt.figure(figsize=(8,5))
        plt.step(df_step['Step'], df_step['GFLOPS'], where='post')
        plt.xlabel('Algorithm Step')
        plt.ylabel('GFLOP/s')
        plt.title(f'Step Performance ({file})')
        plt.grid(True)

        name = file.replace(".csv", ".png")
        plt.savefig(f"plots/{name}")
        plt.close()