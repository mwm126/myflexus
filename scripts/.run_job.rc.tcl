rungen phase {
  { oracle_16cpu_16cl          baseline  0:0     "cpu0.cycle-break 500000000"         }
  { db2v8_tpcc_nort_16cpu_64cl baseline  0:0     "cpu0.cycle-break 500000000"         }

  { apache_16cpu_40cl2         baseline  0:0     "server_cpu0.cycle-break 500000000"  }
  { zeus_16cpu_40cl            baseline  0:0     "server_cpu0.cycle-break 500000000"  }
  { apache_16cpu_40cl3         baseline  0:0     "server_cpu0.cycle-break 500000000"  }
  { zeus_16cpu_40cl3           baseline  0:0     "server_cpu0.cycle-break 500000000"  }

  { db2v8_tpch_qry6_16cpu      baseline  0:0     "cpu0.cycle-break 500000000"         }
  { db2v8_tpch_qry16_16cpu     baseline  0:0     "cpu0.cycle-break 500000000"         }

  { apache_4cpu_spec09         baseline  0:0     "server_cpu0.cycle-break 2000000000" }
  { apache_16cpu_spec09        baseline  0:0     "server_cpu0.cycle-break 2000000000" }


  { cassandra/1cpu             baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/2cpu             baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/4cpu             baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/8cpu             baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/16cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/32cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cassandra/64cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }

  { cloud9/1cpu                baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/2cpu                baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/4cpu                baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/8cpu                baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/16cpu               baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/16cpu               baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/32cpu               baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { cloud9/64cpu               baseline  0-9:0   "cpu0.cycle-break 2000000000" }

  { classification/1cpu        baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/2cpu        baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/4cpu        baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/8cpu        baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/16cpu       baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/32cpu       baseline  0-9:0   "cpu0.cycle-break 2000000000" }
  { classification/64cpu       baseline  0-9:0   "cpu0.cycle-break 2000000000" }

  { nutch/1cpu                 baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { nutch/2cpu                 baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { nutch/4cpu                 baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { nutch/8cpu                 baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { nutch/16cpu                baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { nutch/32cpu                baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }

  { streaming/1cpu             baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/2cpu             baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/4cpu             baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/8cpu             baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/16cpu            baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/32cpu            baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }
  { streaming/64cpu            baseline  0-9:0   "server_cpu0.cycle-break 6000000000" }

  { cloudstone/1cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/2cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/4cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/8cpu            baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/16cpu           baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/32cpu           baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
  { cloudstone/64cpu           baseline  0-9:0   "server_cpu0.cycle-break 2000000000" }
}

rungen flexpoint {
  { flexus_test_app_v9         baseline    0:0   "cpu0.cycle-break 4000000;           instruction-fetch-mode instruction-fetch-trace" }
  { flexus_test_app_x86        baseline    0:0   "cpu0.cycle-break 4000000;           instruction-fetch-mode instruction-fetch-trace" }

  { cassandra/1cpu             baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/2cpu             baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/4cpu             baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/8cpu             baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/16cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/32cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cassandra/64cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }

  { cloud9/1cpu                baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/2cpu                baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/4cpu                baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/8cpu                baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/16cpu               baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/16cpu               baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/32cpu               baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { cloud9/64cpu               baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }

  { classification/1cpu        baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/2cpu        baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/4cpu        baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/8cpu        baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/16cpu       baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/32cpu       baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }
  { classification/64cpu       baseline  0-9:0   "cpu0.cycle-break 25000000;          instruction-fetch-mode instruction-fetch-trace" }

  { nutch/1cpu                 baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { nutch/2cpu                 baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { nutch/4cpu                 baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { nutch/8cpu                 baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { nutch/16cpu                baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { nutch/32cpu                baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }

  { streaming/1cpu             baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/2cpu             baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/4cpu             baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/8cpu             baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/16cpu            baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/32cpu            baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }
  { streaming/64cpu            baseline  0-9:0   "server_cpu0.cycle-break 75000000;   instruction-fetch-mode instruction-fetch-trace" }

  { cloudstone/1cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/2cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/4cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/8cpu            baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/16cpu           baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/32cpu           baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
  { cloudstone/64cpu           baseline  0-9:0   "server_cpu0.cycle-break 25000000;   instruction-fetch-mode instruction-fetch-trace" }
}

array set simics_commands {
  trace_commercial {
    flexus.set "-magic-break:stop_cycle" "2000000000"
      flexus.set-region-interval "1000000000"
      instruction-fetch-mode instruction-fetch-trace
  }
  timing_commercial {
    flexus.set "-magic-break:stop_cycle" "150000"
      flexus.set-region-interval "50000"
      instruction-fetch-mode instruction-fetch-trace
      console_tracker.add-break-string "bash"
  }
  timing_commercial_long {
    flexus.set "-magic-break:stop_cycle" "2050000"
      flexus.set-region-interval "1000000"
      instruction-fetch-mode instruction-fetch-trace
      console_tracker.add-break-string "bash"
  }
}

rungen trace {
  { flexus_test_app_v9          baseline    0:0     $simics_commands(trace_commercial) }
  { flexus_test_app_x86         baseline    0:0     $simics_commands(trace_commercial) }

  { cloud9/1cpu                 baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/2cpu                 baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/4cpu                 baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/8cpu                 baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/16cpu                baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/32cpu                baseline    0:0     $simics_commands(trace_commercial) }
  { cloud9/64cpu                baseline    0:0     $simics_commands(trace_commercial) }

  { classification/1cpu         baseline    0:0     $simics_commands(trace_commercial) }
  { classification/2cpu         baseline    0:0     $simics_commands(trace_commercial) }
  { classification/4cpu         baseline    0:0     $simics_commands(trace_commercial) }
  { classification/8cpu         baseline    0:0     $simics_commands(trace_commercial) }
  { classification/16cpu        baseline    0:0     $simics_commands(trace_commercial) }
  { classification/32cpu        baseline    0:0     $simics_commands(trace_commercial) }
  { classification/64cpu        baseline    0:0     $simics_commands(trace_commercial) }

  { nutch/1cpu                  baseline    0:0     $simics_commands(trace_commercial) }
  { nutch/2cpu                  baseline    0:0     $simics_commands(trace_commercial) }
  { nutch/4cpu                  baseline    0:0     $simics_commands(trace_commercial) }
  { nutch/8cpu                  baseline    0:0     $simics_commands(trace_commercial) }
  { nutch/16cpu                 baseline    0:0     $simics_commands(trace_commercial) }
  { nutch/32cpu                 baseline    0:0     $simics_commands(trace_commercial) }

  { cassandra/1cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/2cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/4cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/8cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/16cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/32cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cassandra/64cpu             baseline    0:0     $simics_commands(trace_commercial) }

  { streaming/1cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/2cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/4cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/8cpu              baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/16cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/32cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { streaming/64cpu             baseline    0:0     $simics_commands(trace_commercial) }

  { cloudstone/1cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/2cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/4cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/8cpu             baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/16cpu            baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/32cpu            baseline    0:0     $simics_commands(trace_commercial) }
  { cloudstone/64cpu            baseline    0:0     $simics_commands(trace_commercial) }
}

rungen timing {
  { flexus_test_app_v9          baseline    0:18-28  $simics_commands(timing_commercial) }
  { flexus_test_app_x86         baseline    0:12-24  $simics_commands(timing_commercial) }

  { cloud9/1cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/2cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/4cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/8cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/16cpu                baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/32cpu                baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloud9/64cpu                baseline  0-9:5-84   $simics_commands(timing_commercial) }

  { nutch/1cpu                  baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { nutch/2cpu                  baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { nutch/4cpu                  baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { nutch/8cpu                  baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { nutch/16cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { nutch/32cpu                 baseline  0-9:5-84   $simics_commands(timing_commercial) }

  { cassandra/1cpu              baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/2cpu              baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/4cpu              baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/8cpu              baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/16cpu             baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/32cpu             baseline  0-9:5-84   $simics_commands(timing_commercial_long) }
  { cassandra/64cpu             baseline  0-9:5-84   $simics_commands(timing_commercial_long) }

  { classification/1cpu         baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/2cpu         baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/4cpu         baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/8cpu         baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/16cpu        baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/32cpu        baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { classification/64cpu        baseline  0-9:5-84   $simics_commands(timing_commercial) }

  { streaming/1cpu              baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/2cpu              baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/4cpu              baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/8cpu              baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/16cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/32cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { streaming/64cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }

  { cloudstone/1cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/2cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/4cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/8cpu             baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/16cpu            baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/32cpu            baseline  0-9:5-84   $simics_commands(timing_commercial) }
  { cloudstone/64cpu            baseline  0-9:5-84   $simics_commands(timing_commercial) }
}
