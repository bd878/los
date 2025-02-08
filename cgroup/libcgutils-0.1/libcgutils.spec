Name:           libcgutils
Version:        0.1
Release:        1%{?dist}
Summary:        Library to work with cgroupfs

License:        GPLv2
URL:            https://github.com/bd878/los/cgroup/cgutils
Source0:        https://github.com/bd878/los/releases/%{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
Requires:	bash
BuildArch:	x86_64

%description
Library that works with cgroupfs

%global debug_package %{nil}

%prep
%setup -q 

%build
make %{?_smp_mflags}


%install
%make_install


%files
/usr/local/lib/libcgutils.so
/usr/local/include/cgroup_utils.h
%license LICENSE


%changelog
* Fri Feb 07 2025 bd878 <admin@make-init.ru> - 0.1
- init
