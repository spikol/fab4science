#!/usr/bin/env python3
# generate_plots.py : use the matplotlib library to generate graphical plots of data

import argparse
import matplotlib.pyplot as plt
import numpy as np

################################################################
def _make_subplots(rows):
    # Create a plot or set of plots at a common figure size and resolution.
    fig, ax = plt.subplots(nrows=rows)
    fig.set_dpi(160)
    fig.set_size_inches((8,6))
    return fig, ax

################################################################
def make_raw_plot(t_axis, y_axis):
    # create a plot of the raw range data
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, y_axis, 'x')
    ax.set_title("Raw Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Ping Time (microseconds)")
    fig.savefig('plots/raw_sonar.png')

################################################################
def make_median_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, 'x', label='calibrated')
    ax.plot(t_axis, y_axis, label='median-filtered')
    ax.set_title("Median-Filtered Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/median.png')

################################################################
def make_nozero_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, 'x', label='calibrated')
    ax.plot(t_axis, y_axis, label='zero-suppressed')
    ax.set_title("Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/nozero.png')

################################################################
def make_smoothed_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='smoothed')
    ax.set_title("Smoothed Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/smoothed.png')

################################################################
def make_lowpass_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='lowpass')
    ax.set_title("Low-pass Filtered Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/lowpass.png')

################################################################
def make_highpass_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='highpass')
    ax.set_title("High-pass Filtered Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/highpass.png')

################################################################
def make_bandpass_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='bandpass')
    ax.set_title("Band-pass Filtered Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/bandpass.png')

################################################################
def make_bandstop_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='bandstop')
    ax.set_title("Band-stop Filtered Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/bandstop.png')

################################################################
def make_ring_median_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, calib_axis, label='zero-suppressed')
    ax.plot(t_axis, y_axis, label='median-filtered')
    ax.set_title("Median Filtering of Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Distance (cm)")
    ax.legend()
    fig.savefig('plots/ring-median.png')

################################################################
def make_differenced_plot(t_axis, y_axis, calib_axis):
    fig, ax = _make_subplots(1)   # Create a figure containing a single set of axes.
    ax.plot(t_axis, y_axis, color='C1')
    ax.set_title("Finite Difference of Zero-Suppressed Range Data")
    ax.set_xlabel("Time (seconds)")
    ax.set_ylabel("Velocity (cm/sec)", color='C1')

    right_ax = ax.twinx()
    right_ax.plot(t_axis, calib_axis)
    right_ax.set_ylabel("Position (cm)")
    fig.savefig('plots/differenced.png')
    
################################################################    
def make_traj_plot(t_axis, data, cm):
    pos = data[:,0]
    vel = data[:,1]
    acc = data[:,2]

    fig, axes = _make_subplots(3) # Create a triple-axis figure.
    pos_axis = axes[0]
    pos_axis.plot(t_axis, cm, 'x', label="Input", color="C0")
    pos_axis.plot(t_axis, pos, label="Position", color="C1")
    pos_axis.set_title("Quadratic Trajectory Fit to Range Data")
    # pos_axis.set_xlabel("Time (seconds)")
    pos_axis.set_ylabel("Distance (cm)")
    pos_axis.legend()

    vel_axis = axes[1]
    # vel_axis.set_ylim(-2, 11)
    vel_axis.plot(t_axis, vel, label="Velocity", color="C1")
    vel_axis.set_title("Velocity")
    vel_axis.set_xlabel("Time (seconds)")
    vel_axis.set_ylabel("cm/sec")

    acc_axis = axes[2]
    # acc_axis.set_ylim(-2, 2)
    acc_axis.plot(t_axis, acc, label="Acceleration", color="C1")
    acc_axis.set_title("Acceleration")
    acc_axis.set_xlabel("Time (seconds)")
    acc_axis.set_ylabel("cm/sec/sec")

    fig.tight_layout()
    fig.savefig('plots/trajfit.png')
    
################################################################
if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('filename', default="data/raw_waving.dat", nargs="?", help='Original raw range data file name.')
    args = parser.parse_args()
    
    # read in the data files as numpy arrays; these are then shared among
    # various plot-generating functions

    # raw range values in microseconds
    raw_range = np.loadtxt(args.filename)

    # raw range values converted to centimeters
    calibrated = np.loadtxt("data/calibrated.dat")

    # median of three applied to raw data, units are centimeters
    median = np.loadtxt("data/median.dat")

    # zero-suppression applied to range values, units are centimeters
    no_zero = np.loadtxt("data/nozero.dat")

    # smoothed zero-suppressed values, units are centimeters
    smoothed = np.loadtxt("data/smoothed.dat")

    # lowpass zero-suppressed values, units are centimeters
    lowpass = np.loadtxt("data/lowpass.dat")

    # highpass zero-suppressed values, units are centimeters
    highpass = np.loadtxt("data/highpass.dat")

    # bandpass zero-suppressed values, units are centimeters
    bandpass = np.loadtxt("data/bandpass.dat")

    # bandstop zero-suppressed values, units are centimeters
    bandstop = np.loadtxt("data/bandstop.dat")
    
    # finite-differencing velocity estimate applied to zero-suppressed ring buffer, units are cm/sec
    differenced = np.loadtxt("data/differenced.dat")

    # median filter applied to zero-suppressed ring buffer, units are cm
    ring_median = np.loadtxt("data/ring-median.dat")

    # quadratic trajectory fit of zero-suppressed ring buffer, units are cm, cm/sec, cm/sec/sec
    trajfit = np.loadtxt("data/trajfit.dat")

    # all files are assumed the same length and sampling rate; this produces a time axis
    delta_t = 0.1
    samples = len(raw_range)
    t_axis = np.arange(0, samples*delta_t, delta_t)

    # produce plots of various combinations
    make_raw_plot(t_axis, raw_range)
    make_median_plot(t_axis, median, calibrated)
    make_nozero_plot(t_axis, no_zero, calibrated)
    make_smoothed_plot(t_axis, smoothed, no_zero)
    make_lowpass_plot(t_axis, lowpass, no_zero)
    make_highpass_plot(t_axis, highpass, no_zero)
    make_bandpass_plot(t_axis, bandpass, no_zero)
    make_bandstop_plot(t_axis, bandstop, no_zero)
    make_ring_median_plot(t_axis, ring_median, no_zero)
    make_differenced_plot(t_axis, differenced, no_zero)
    make_traj_plot(t_axis, trajfit, no_zero)
